
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <rtl/string.h>

#define NK_USE_UNQUALIFIED_NAMES

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

/* EDL description of the LightsGPIO entity. */
#include <traffic_light/LightsGPIO.edl.h>
#include <traffic_light/IDiagnostics.idl.h>

#include <assert.h>

#define CLR_RED      "\x1b[91m"
#define CLR_YELLOW   "\x1b[93m"
#define CLR_GREEN    "\x1b[92m"
#define CLR_RESET    "\x1b[0m"


static const char EntityName[] = "LightsGPIO";

/* Type of interface implementing object. */
typedef struct IModeImpl {
    struct traffic_light_IMode base;     /* Base interface of object */
    rtl_uint32_t step;                   /* Extra parameters */
} IModeImpl;


/* Проще всего запомнить состояние сигналов так */
static uint32_t lights_bitfield = 0;

/* Mode method implementation. */
static nk_err_t FMode_impl(struct traffic_light_IMode *self,
                          const struct traffic_light_IMode_FMode_req *req,
                          const struct nk_arena *req_arena,
                          traffic_light_IMode_FMode_res *res,
                          struct nk_arena *res_arena)
{
    IModeImpl *impl = (IModeImpl *)self;
    /**
     * Increment value in control system request by
     * one step and include into result argument that will be
     * sent to the control system in the lights gpio response.
     */

    // Запомнить состояние 
    lights_bitfield = (req->value)&0xF0F;
    res->result = req->value + impl->step;
    return NK_EOK;
}

/**
 * IMode object constructor.
 * step is the number by which the input value is increased.
 */
static struct traffic_light_IMode *CreateIModeImpl(rtl_uint32_t step)
{
    /* Table of implementations of IMode interface methods. */
    static const struct traffic_light_IMode_ops ops = {
        .FMode = FMode_impl
    };

    /* Interface implementing object. */
    static struct IModeImpl impl = {
        .base = {&ops}
    };

    impl.step = step;

    return &impl.base;
}

/* функция оценки корректности установленного сигнала */
struct check_lights_result{
    uint32_t code;
    char msg[256];
};

#define LIGHTS_OK 42
#define LIGHTS_FORBIDDEN 13
#define LIGHTS_ALLOFF 99
#define LIGHST_DOUBTFUL 63
struct check_lights_result check_lights(){

    struct check_lights_result clr = {.code = LIGHTS_OK};
    //memset(clr.msg, 0, 256);
    strncpy(clr.msg,"all ok",256);

    /* Если  два зелёных на пересечении - запрещенное */
    if ((lights_bitfield&0x404) == 0x404){
        clr.code = LIGHTS_FORBIDDEN;
        strncpy(clr.msg,"forbidden state",256);
        return clr;
    }
     

    /* если вместе с зелёным горит чтото еще - сомнительное*/
    /* но зелёный может мигать */
    if ((lights_bitfield&0x4)&&(lights_bitfield&0x3)){
        clr.code = LIGHST_DOUBTFUL;
        strncpy(clr.msg,"seems doubtful state",256);
        return clr;
    }
    if ((lights_bitfield&0x400)&&(lights_bitfield&0x300)){
        clr.code = LIGHST_DOUBTFUL;
        strncpy(clr.msg,"seems doubtful state",256);
        return clr;
    } 

    /* если всё выключено */
    if (!(lights_bitfield&0xF0F)){
        clr.code = LIGHTS_ALLOFF;  
        strncpy(clr.msg,"lights all off",256);
        return clr;
    }

    /* нерегулируемый */
    if (lights_bitfield == 0x808){
        clr.code = LIGHTS_ALLOFF;  
        strncpy(clr.msg,"blinking yellow",256);
        return clr;
    }

    return clr;
}

/* Консольный имитатор светофора */
void lights_stub(){
    uint8_t dir1 = lights_bitfield&0xF;
    uint8_t dir2 = ((lights_bitfield&0xF00)>>8);

    char dir1_str[128]; 
    char dir2_str[128];

    bzero(dir1_str,32),bzero(dir2_str,32);

    int next = 0;

    /* dir1 */
    if (dir1&0x8){
        next += sprintf( dir1_str+next, "%s", "BLINK:" );
    }else{
        next += sprintf( dir1_str+next, "%s", "SOLID:" );
    }

    if (dir1&0x1){
        next += sprintf( dir1_str+next, "%s%c", CLR_RED, 'R' );
    }

    if (dir1&0x2){
        next += sprintf( dir1_str+next, "%s%c", CLR_YELLOW, 'Y' );
    }

    if (dir1&0x4){
        next += sprintf( dir1_str+next, "%s%c", CLR_GREEN, 'G' );
    }
    sprintf( dir1_str+next, "%s", CLR_RESET );

    /* dir2 */
    next = 0;
    if (dir2&0x8){
        next += sprintf( dir2_str+next, "%s", "BLINK:" );
    }else{
        next += sprintf( dir2_str+next, "%s", "SOLID:" );
    }

    if (dir2&0x1){
        next += sprintf( dir2_str+next, "%s%c", CLR_RED, 'R' );
    }

    if (dir2&0x2){
        next += sprintf( dir2_str+next, "%s%c", CLR_YELLOW, 'Y' );
    }

    if (dir2&0x4){
        next += sprintf( dir2_str+next, "%s%c", CLR_GREEN, 'G' );
    }
    sprintf( dir2_str+next, "%s", CLR_RESET );

    fprintf(stderr, "[%s] Dir1: %s Dir2: %s\n", EntityName, dir1_str, dir2_str);

}


/* вынесем код обслуги IPC в отдельные функции */
/* Дескриптор соединения lights_gpio_connection (входящее) */
typedef struct {
    NkKosTransport transport;
    ServiceId iid;
    Handle handle;
    LightsGPIO_entity_req req;
    char req_buffer[LightsGPIO_entity_req_arena_size];
    struct nk_arena req_arena;
    LightsGPIO_entity_res res;
    char res_buffer[LightsGPIO_entity_res_arena_size];
    struct nk_arena res_arena;
    CMode_component component;
    LightsGPIO_entity entity;
} CMode_TransportDescriptor;

/* Инициализатор дескриптора lights_gpio_connection (входящее) */
void lights_gpio_connection_init(CMode_TransportDescriptor *CMode_td){

    /* Get lights gpio IPC handle of "lights_gpio_connection". */
    CMode_td->handle = ServiceLocatorRegister("lights_gpio_connection", NULL, 0, &CMode_td->iid);
    assert(CMode_td->handle != INVALID_HANDLE);

    /* Initialize transport to control system. */
    NkKosTransport_Init(&CMode_td->transport, CMode_td->handle, NK_NULL, 0);

    // Структуры запрос/ответ
    struct nk_arena req_arena = NK_ARENA_INITIALIZER(CMode_td->req_buffer,
                                        CMode_td->req_buffer + sizeof(CMode_td->req_buffer));

    
    struct nk_arena res_arena = NK_ARENA_INITIALIZER(CMode_td->res_buffer,
                                        CMode_td->res_buffer + sizeof(CMode_td->res_buffer));

    CMode_td->req_arena = req_arena;
    CMode_td->res_arena = res_arena;

    // диспетчер компонент
    CMode_component_init(&CMode_td->component, CreateIModeImpl(0x1000000));

    /* Initialize lights gpio entity dispatcher. */
    LightsGPIO_entity_init(&CMode_td->entity, &CMode_td->component);
}

/* Процессор  дескриптора lights_gpio_connection (входящее) */
void lights_gpio_connection_loop(CMode_TransportDescriptor *CMode_td){
    /* Flush request/response buffers. */
    nk_req_reset(&CMode_td->req);
    nk_arena_reset(&CMode_td->req_arena);
    nk_arena_reset(&CMode_td->res_arena);

    /* Wait for request to lights gpio entity. */
    if (nk_transport_recv(&CMode_td->transport.base,
                            &CMode_td->req.base_,
                            &CMode_td->req_arena) != NK_EOK) {
        fprintf(stderr, "[%s] nk_transport_recv error\n", EntityName);
    } else {
        /**
         * Handle received request by calling implementation Mode_impl
         * of the requested Mode interface method.
         */
        LightsGPIO_entity_dispatch(&CMode_td->entity, &CMode_td->req.base_, &CMode_td->req_arena,
                                    &CMode_td->res.base_, &CMode_td->res_arena);
    }

    /* Send response. */
    if (nk_transport_reply(&CMode_td->transport.base,
                            &CMode_td->res.base_,
                            &CMode_td->res_arena) != NK_EOK) {
        fprintf(stderr, "[%s] nk_transport_reply error\n", EntityName);
    }
}

/* Теперь соединение diagnostics_connection (исходящее) */
typedef struct{
    struct IDiagnostics_proxy proxy;
    NkKosTransport transport;
    nk_iid_t riid;
    Handle handle;
    IDiagnostics_DMessage_req req;
    char req_buffer[IDiagnostics_DMessage_req_arena_size];
    struct nk_arena req_arena;
    IDiagnostics_DMessage_req res;
    //char res_buffer[IDiagnostics_DMessage_res_arena_size];
    //struct nk_arena res_arena;
}CDMmessage_TransportDescriptor;

/* Инициализатор дескриптора diagnostics_connection (исходящее) */
void diagnostics_connection_init(CDMmessage_TransportDescriptor *CDMmessage_td){
    CDMmessage_td->handle = ServiceLocatorConnect("diagnostics_connection");
    assert(CDMmessage_td->handle != INVALID_HANDLE);
    NkKosTransport_Init(&CDMmessage_td->transport, CDMmessage_td->handle, NK_NULL, 0);
    CDMmessage_td->riid = ServiceLocatorGetRiid(CDMmessage_td->handle, "diagnostics.dmessage");
    assert(CDMmessage_td->riid  != INVALID_RIID);
    IDiagnostics_proxy_init(&CDMmessage_td->proxy,
                                &CDMmessage_td->transport.base,
                                CDMmessage_td->riid);
    struct nk_arena arena = NK_ARENA_INITIALIZER(
                                CDMmessage_td->req_buffer,
                                CDMmessage_td->req_buffer + 
                                sizeof(CDMmessage_td->req_buffer));
    CDMmessage_td->req_arena = arena;
}

void diagnostics_connection_loop(CDMmessage_TransportDescriptor *CDMmessage_td, 
                                    uint32_t code,
                                    const char * msg){
    nk_err_t rc;

    nk_req_reset(&CDMmessage_td->req);
    nk_arena_reset(&CDMmessage_td->req_arena);

    CDMmessage_td->req.code = code;

    rtl_size_t msg_len = rtl_strlen(msg)+1;
    nk_char_t *str = nk_arena_alloc(
                    nk_char_t,
                    &CDMmessage_td->req_arena,
                    &CDMmessage_td->req.msg,
                    msg_len);
    if (str == RTL_NULL)
    {
        fprintf(
            stderr,
            "[%s]: Error: can`t allocate memory in arena!\n",
            EntityName);
    }

    rtl_strncpy(str, msg, (rtl_size_t) msg_len);

    if ((rc = IDiagnostics_DMessage(&CDMmessage_td->proxy.base,
                            &CDMmessage_td->req,
                            &CDMmessage_td->req_arena,
                            &CDMmessage_td->res,
                            NULL)
    ) != NK_EOK){
        fprintf(stderr, "[%s] IDiagnostics_DMessage error %d\n", EntityName, rc);
    }
}

/* Lights GPIO entry point. */
int main(void)
{
    
    fprintf(stderr, "[%s] started\n", EntityName);

    /* Заинитить кишки своего API */
    CMode_TransportDescriptor cs_td;
    lights_gpio_connection_init(&cs_td);

    fprintf(stderr, "[%s] API transport inited\n", EntityName);


    CDMmessage_TransportDescriptor cdm_td;
    diagnostics_connection_init(&cdm_td);


    /* Dispatch loop implementation. */
    do
    {
        /* прокручивать диспетчинг своего API */
        lights_gpio_connection_loop(&cs_td);

        /* Типа имитация светофора */
        lights_stub();

        /* Типа проверка состояния */
        struct check_lights_result clr = check_lights();
        diagnostics_connection_loop(&cdm_td, clr.code, clr.msg);
        
        
    }
    while (true);

    return EXIT_SUCCESS;
}

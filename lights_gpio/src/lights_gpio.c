
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <rtl/string.h>

#define NK_USE_UNQUALIFIED_NAMES

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

/* EDL description of the LightsGPIO entity. */
#include <traffic_light/LightsGPIO.edl.h>
#include <traffic_light/IDiagnostics.idl.h>

#include <assert.h>

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

/* вынесем код обслуги IPC в отдельные функции */
/* Дескриптор соединения lights_gpio_connection */
typedef struct {
    //struct IMode_proxy *proxy;
    //IMode_FMode_req *req;
    //IMode_FMode_res *res;
} CMode_TransportDescriptor;

CMode_TransportDescriptor lights_gpio_connection_init(){

}

void lights_gpio_connection_loop(){

}


/* Lights GPIO entry point. */
int main(void)
{
    nk_err_t rc;
    NkKosTransport transport;
    ServiceId iid;

    fprintf(stderr, "[%s] started\n", EntityName);

    /* Get lights gpio IPC handle of "lights_gpio_connection". */
    Handle handle = ServiceLocatorRegister("lights_gpio_connection", NULL, 0, &iid);
    assert(handle != INVALID_HANDLE);

    /* Initialize transport to control system. */
    NkKosTransport_Init(&transport, handle, NK_NULL, 0);

    /**
     * Prepare the structures of the request to the lights gpio entity: constant
     * part and arena. Because none of the methods of the lights gpio entity has
     * sequence type arguments, only constant parts of the
     * request and response are used. Arenas are effectively unused. However,
     * valid arenas of the request and response must be passed to
     * lights gpio transport methods (nk_transport_recv, nk_transport_reply) and
     * to the lights gpio method.
     */
    LightsGPIO_entity_req req;
    char req_buffer[LightsGPIO_entity_req_arena_size];
    struct nk_arena req_arena = NK_ARENA_INITIALIZER(req_buffer,
                                        req_buffer + sizeof(req_buffer));

    /* Prepare response structures: constant part and arena. */
    LightsGPIO_entity_res res;
    char res_buffer[LightsGPIO_entity_res_arena_size];
    struct nk_arena res_arena = NK_ARENA_INITIALIZER(res_buffer,
                                        res_buffer + sizeof(res_buffer));

    /**
     * Initialize mode component dispatcher. 3 is the value of the step,
     * which is the number by which the input value is increased.
     */
    CMode_component component;
    CMode_component_init(&component, CreateIModeImpl(0x1000000));

    /* Initialize lights gpio entity dispatcher. */
    LightsGPIO_entity entity;
    LightsGPIO_entity_init(&entity, &component);

    /* Так, а теперь подключение к Diagnostics */
    struct IDiagnostics_proxy d_proxy;
    Handle d_handle = ServiceLocatorConnect("diagnostics_connection");
    assert(d_handle != INVALID_HANDLE);

    NkKosTransport d_transport;  // Нужен новый?
    NkKosTransport_Init(&d_transport, d_handle, NK_NULL, 0);

    /* получить идентификатор интерфейса */
    nk_iid_t riid = ServiceLocatorGetRiid(d_handle, "diagnostics.dmessage");
    assert(riid != INVALID_RIID);

    /* Сделапть proxy - объект */
    IDiagnostics_proxy_init(&d_proxy, &d_transport.base, riid);

    /* определить структуры запроса и ответа */
    IDiagnostics_DMessage_req d_req;
    IDiagnostics_DMessage_res d_res;

    char reqBuffer[IDiagnostics_DMessage_req_arena_size];
    struct nk_arena d_arena = NK_ARENA_INITIALIZER(
                                reqBuffer, reqBuffer + sizeof(reqBuffer));

    /* Dispatch loop implementation. */
    do
    {
        /* Flush request/response buffers. */
        nk_req_reset(&req);
        nk_arena_reset(&req_arena);
        nk_arena_reset(&res_arena);

        /* Wait for request to lights gpio entity. */
        if (nk_transport_recv(&transport.base,
                              &req.base_,
                              &req_arena) != NK_EOK) {
            fprintf(stderr, "nk_transport_recv error\n");
        } else {
            /**
             * Handle received request by calling implementation Mode_impl
             * of the requested Mode interface method.
             */
            traffic_light_LightsGPIO_entity_dispatch(&entity, &req.base_, &req_arena,
                                        &res.base_, &res_arena);
        }

        /* Send response. */
        if (nk_transport_reply(&transport.base,
                               &res.base_,
                               &res_arena) != NK_EOK) {
            fprintf(stderr, "nk_transport_reply error\n");
        }

        struct check_lights_result clr = check_lights();
        d_req.code = clr.code;
        
        nk_req_reset(&d_req);
        nk_arena_reset(&d_arena);

        rtl_size_t msg_len = rtl_strlen(clr.msg)+1;
        nk_char_t *str = nk_arena_alloc(
                        nk_char_t,
                        &d_arena,
                        &d_req.msg,
                        msg_len);
        if (str == RTL_NULL)
        {
            fprintf(
                stderr,
                "[%s]: Error: can`t allocate memory in arena!\n",
                EntityName);
        }

        rtl_strncpy(str, clr.msg, (rtl_size_t) msg_len);

        if ((rc = IDiagnostics_DMessage(&d_proxy.base,
                                &d_req,
                                &d_arena,
                                &d_res,
                                NULL)
        ) != NK_EOK){
            fprintf(stderr, "[%s] IDiagnostics_DMessage error %d\n", EntityName, rc);
        }
    }
    while (true);

    return EXIT_SUCCESS;
}

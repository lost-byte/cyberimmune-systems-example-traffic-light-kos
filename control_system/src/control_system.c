
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#define NK_USE_UNQUALIFIED_NAMES

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

/* Description of the lights gpio interface used by the `ControlSystem` entity. */
#include <traffic_light/ControlSystem.edl.h>
#include <traffic_light/IMode.idl.h>
#include <traffic_light/ICSMode.idl.h>

#include <assert.h>

//#define MODES_NUM 13
static const char EntityName[] = "Control System";

/* Перечисление - состояния */
typedef enum {
    TL_MODE_MANUAL,             // ручное управление
    TL_MODE_UNREG,              // мигающий желтый в обоих направлениях
    TL_MODE_GREEN1,             // 1- зеленый, 2 - красный
    TL_MODE_TO_GREEN2_1,        // переход 1- мигает зелёный 2- мигает красный
    TL_MODE_TO_GREEN2_2,        // переход оба желтые
    TL_MODE_GREEN2,             // 1 - красный, 2 - зелёный
    TL_MODE_TO_GREEN1_1,        // переход 1- мигает красный 2- мигает зелёный
    TL_MODE_TO_GREEN1_2,        // переход оба желтые
} TL_MODES;

#define TL_MODES_CNT 8

/* массив инициализаторов таймера состояния */
uint16_t state_times[TL_MODES_CNT] = {
    0xFFFF,                             // если мы в ручно управлении - там и оставаться
    30,                                 // нерегулируемый
    30,                                 // направление 1
    5,                                  // переход к направлению 2 шаг 1
    3,                                  // переход к направлению 2 шаг 2
    30,                                 // направление 2
    5,                                  // переход к направлкнию 1 шаг 1
    3                                   // переход к направлению 1 шаг 2
};

// таблица переходов
TL_MODES const state_transitions[TL_MODES_CNT] = {
    TL_MODE_UNREG,                      // выходя из ручного переходим в нерегулируемый
    TL_MODE_TO_GREEN1_1,                // из нерегулируемого начинаем включать зеленый 1
    TL_MODE_TO_GREEN2_1,                // из зеленого 1 начинаем включать зеленый 2
    TL_MODE_TO_GREEN2_2,                // продолжаем включать зеленый 2
    TL_MODE_GREEN2,                     // включаем зелёный 2
    TL_MODE_TO_GREEN1_1,                // из зеленого 2 начинаем включать зеленый 1
    TL_MODE_TO_GREEN1_2,                // продолжаем включать зеленый 1
    TL_MODE_GREEN1                      // включаем зелёный 1
};

/* таблица управляющих кодов для LightsGPIO */
uint32_t const state_LightsGPIO_codes[TL_MODES_CNT] = {
    0,                                                  // Ручное управление, код спускается сверху
    IMode_Direction1Yellow + IMode_Direction2Yellow+    // нерегулируемый
    IMode_Direction1Blink  + IMode_Direction2Blink,
    IMode_Direction1Green  + IMode_Direction2Red,       // направление 1
    IMode_Direction1Green  + IMode_Direction2Red+       // переход к направлению 2 шаг 1
    IMode_Direction1Blink  + IMode_Direction2Blink,
    IMode_Direction1Yellow + IMode_Direction2Yellow,    // переход к направлению 2 шаг 2
    IMode_Direction1Red    + IMode_Direction2Green,     // направление 2
    IMode_Direction1Red    + IMode_Direction2Green+     // переход к направлкнию 1 шаг 1
    IMode_Direction1Blink  + IMode_Direction2Blink,
    IMode_Direction1Yellow + IMode_Direction2Yellow,    // переход к направлению 1 шаг 2
};

static uint32_t IMode_manualSet = 0;

/* Структура машины состояния */
typedef struct{
    TL_MODES mode;              // код состояния
    uint16_t timer_secs;        // таймер 
    bool entered;               // признак что произошел вход в состояние
} cs_state;

/* Текущее состояние */
static cs_state current_state = {.mode = TL_MODE_UNREG, .timer_secs = 0, .entered=false};

/* структура - описатель кишок IPC с LightsGPIO */
typedef struct {
    struct IMode_proxy *proxy;
    IMode_FMode_req *req;
    IMode_FMode_res *res;
} TransportDescriptor;

/* инициализатор описателя кишок IPC с LightsGPIO */
#define DESCR_INIT(proxyIn, reqIn, resIn) \
{                                           \
    .proxy = proxyIn,                       \
    .req = reqIn,                           \
    .res = resIn,                           \
}

/* Обертка IPC к  LightsGPIO */
int LightsGPIO_send(TransportDescriptor *td, uint32_t mode){

    td->req->value = mode;
    if (IMode_FMode(&td->proxy->base, td->req, NULL, td->res, NULL) == rcOk)
    {
        
        fprintf(stderr,"[%s] FMode(0x%08x)=0x%08x\n", EntityName, mode, (int) td->res->result);
        return 0;
    }
    
    
    fprintf(stderr, "[%s] Failed to call traffic_light.Mode.Mode()\n", EntityName);
    return -1;
}

/* обобщенная функция машины состояний */
void state_control(TransportDescriptor * td){
    while(true){
    /* Уже вошли в состояние? */
    if (!current_state.entered){
        /*
        *  Еще не вошли в состояние, выполнить действия по входу 
        */
        fprintf(stderr, "[%s] Enter state: %u\n", EntityName, current_state.mode);
        // установить таймер
        current_state.timer_secs = state_times[current_state.mode];

        fprintf(stderr, "[%s] State time: %u\n", EntityName, current_state.timer_secs);

        /* в ручном режиме берем состояние из переменной IMode_manualSet */
        uint32_t LightsGPIO_code;
        if (current_state.mode == TL_MODE_MANUAL)
            LightsGPIO_code = IMode_manualSet;
        else
            LightsGPIO_code = state_LightsGPIO_codes[current_state.mode];
        // передать режим в LightsGPIO
        LightsGPIO_send(td, 
            LightsGPIO_code
        );

        

        current_state.entered = true;   // флаг "уже вошли"        
    }else{
        /*
        *   Уже в состоянии, обработать таймер
        */
        //fprintf(stderr, "[%s] State time: %u\n", EntityName, current_state.timer_secs);

        // если таймер выключен, значит так и остаёмся в режиме
        if (current_state.timer_secs!=0xFFFF){

            // истёк ли таймер?
            if (current_state.timer_secs>0){
                current_state.timer_secs--;  // нет, истекаем
            }else{
                // да, перейти в следующее состояние
                current_state.mode = state_transitions[current_state.mode];
                fprintf(stderr, "[%s] Switch to state: %u\n", EntityName, current_state.mode);
                current_state.entered = false;
            }
        }
    }
    sleep(1);
    }
}

/* структура - описатель кишок IPC с Communication (входящее) */
typedef struct {
    NkKosTransport transport;
    ServiceId iid;
    Handle handle;
    ControlSystem_entity_req req;
    char req_buffer[ControlSystem_entity_req_arena_size];
    struct nk_arena req_arena;
    ControlSystem_entity_res res;
    char res_buffer[ControlSystem_entity_res_arena_size];
    struct nk_arena res_arena;
    ControlSystem_component component;
    ControlSystem_entity entity;
} CCSMode_TransportDescriptor;

/* Реализация метода  ICSMode */
static nk_err_t CSMode(struct ICSMode *self,
                        const
                        struct ICSMode_CSMode_req *req,
                        const
                        struct nk_arena *req_arena,
                        struct CSMode_CSMode_res *res,
                        struct nk_arena *res_arena)
{
    
    uint8_t mode=req->modeReq.mode;
    uint16_t manualSet=req->modeReq.manualSet;

    nk_uint32_t times_len = 0;    
    nk_uint16_t *times = nk_arena_get(
                            nk_char_t, req_arena, &(req->modeReq.times), &times_len);

    /* Напечатать в лог что принято */
    fprintf(stderr, "[%s]: Mode[%u], manualset:0x%08x times:(", EntityName, mode, manualSet);
    for (int i =0; i<(times_len/2-1); i++){
        fprintf(stderr,"%u,", times[i]);
    }
    fprintf(stderr,"%u)\n", times[times_len/2-1]);

    /* воткнуть новое состояние */
    switch(mode){
        case 1:
        /* ручное управление */
        current_state.entered = false;
        current_state.mode = TL_MODE_MANUAL;
        state_times[0] = 0xFFFF;
        IMode_manualSet = manualSet;
        break;
        case 2:
        /* автоматическое управление */
        current_state.entered = false;
        current_state.mode = TL_MODE_TO_GREEN1_1;
        for (int i =0; i<(times_len/2-1); i++){
            state_times[i] = times[i];
        }
        break;
        case 3:
        /* нерегулируемы режим */
        current_state.entered = false;
        current_state.mode = TL_MODE_UNREG;
        state_times[1] = 0xFFFF;
        break;
        default:
        break;
    }


    return NK_EOK;
}

static struct ICSMode *CreateICSModeImpl()
{
    /* Table of implementations of IMode interface methods. */
    static const struct ICSMode_ops ops = {
        .CSMode = CSMode
    };

    static struct ICSMode obj =
    {
        .ops = &ops
    };

    return &obj;
}

/* Инициализатор дескриптора control_system_connection (входящее)*/
void cs_connection_init(CCSMode_TransportDescriptor *CCSMode_td){

    /* Get lights gpio IPC handle of "control_system_connection". */
    CCSMode_td->handle = ServiceLocatorRegister("control_system_connection", NULL, 0, &CCSMode_td->iid);
    assert(CCSMode_td->handle != INVALID_HANDLE);

    /* Initialize transport to control system. */
    NkKosTransport_Init(&CCSMode_td->transport, CCSMode_td->handle, NK_NULL, 0);

    // Структуры запрос/ответ
    struct nk_arena req_arena = NK_ARENA_INITIALIZER(CCSMode_td->req_buffer,
                                        CCSMode_td->req_buffer + sizeof(CCSMode_td->req_buffer));

    
    struct nk_arena res_arena = NK_ARENA_INITIALIZER(CCSMode_td->res_buffer,
                                        CCSMode_td->res_buffer + sizeof(CCSMode_td->res_buffer));

    CCSMode_td->req_arena = req_arena;
    CCSMode_td->res_arena = res_arena;

    // диспетчер компонент
    CCSMode_component_init(&CCSMode_td->component, CreateICSModeImpl());

    /* Initialize lights gpio entity dispatcher. */
    ControlSystem_entity_init(&CCSMode_td->entity, &CCSMode_td->component);
}

/* Процессор  дескриптора control_system_connection  (входящее) */
void cs_connection_loop(CCSMode_TransportDescriptor *CCSMode_td){
    /* Flush request/response buffers. */
    nk_req_reset(&CCSMode_td->req);
    nk_arena_reset(&CCSMode_td->req_arena);
    nk_arena_reset(&CCSMode_td->res_arena);

    /* Wait for request to lights gpio entity. */
    if (nk_transport_recv(&CCSMode_td->transport.base,
                            &CCSMode_td->req.base_,
                            &CCSMode_td->req_arena) != NK_EOK) {
        fprintf(stderr, "[%s] nk_transport_recv error\n", EntityName);
    } else {
        /**
         * Handle received request by calling implementation Mode_impl
         * of the requested Mode interface method.
         */
        ControlSystem_entity_dispatch(&CCSMode_td->entity, &CCSMode_td->req.base_, &CCSMode_td->req_arena,
                                    &CCSMode_td->res.base_, &CCSMode_td->res_arena);
    }

    /* Send response. */
    if (nk_transport_reply(&CCSMode_td->transport.base,
                            &CCSMode_td->res.base_,
                            &CCSMode_td->res_arena) != NK_EOK) {
        fprintf(stderr, "[%s] nk_transport_reply error\n", EntityName);
    }
}

/* Control system entity entry point. */
int main()
{
    NkKosTransport transport;
    struct IMode_proxy proxy;

    fprintf(stderr, "[%s] started\n", EntityName);

    /**
     * Get the LightsGPIO IPC handle of the connection named
     * "lights_gpio_connection".
     */
    Handle handle = ServiceLocatorConnect("lights_gpio_connection");
    assert(handle != INVALID_HANDLE);

    /* Initialize IPC transport for interaction with the lights gpio entity. */
    NkKosTransport_Init(&transport, handle, NK_NULL, 0);

    /**
     * Get Runtime Interface ID (RIID) for interface traffic_light.Mode.mode.
     * Here mode is the name of the traffic_light.Mode component instance,
     * traffic_light.Mode.mode is the name of the Mode interface implementation.
     */
    nk_iid_t riid = ServiceLocatorGetRiid(handle, "lightsGpio.mode");
    assert(riid != INVALID_RIID);

    /**
     * Initialize proxy object by specifying transport (&transport)
     * and lights gpio interface identifier (riid). Each method of the
     * proxy object will be implemented by sending a request to the lights gpio.
     */
    traffic_light_IMode_proxy_init(&proxy, &transport.base, riid);

    /* Request and response structures */
    IMode_FMode_req req;
    IMode_FMode_res res;

    
    
    /* Засунуть заиниченые кишки транспорта в одну структуру для передачи в функцию */
    TransportDescriptor td = DESCR_INIT(
                                &proxy, &req, &res);

    fprintf(stderr, "[%s] IPC to LightsGPIO inited...\n", EntityName);

    /* Инициализация входящего соединения от Communication */
    CCSMode_TransportDescriptor CCSMode_td;
    cs_connection_init(&CCSMode_td);


    /*  поскольку обслуга IPC локается вынесем машину состояний в нить*/
    pthread_t thread;
    int retCode = pthread_create(&thread, NULL, state_control, &td);
    if (retCode != 0)
    {
        fprintf(stderr, "[%s] pthread_create returns error %d\n", EntityName, retCode);
        return false;
    }

    /*
    *  основной цикл, обслуга машины состояний
    */
    while(true){
        //state_control(&td);
        cs_connection_loop(&CCSMode_td);
        
        
    }


    return EXIT_SUCCESS;
}

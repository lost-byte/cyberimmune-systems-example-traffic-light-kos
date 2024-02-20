
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define NK_USE_UNQUALIFIED_NAMES

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

/* Description of the lights gpio interface used by the `ControlSystem` entity. */
#include <traffic_light/ControlSystem.edl.h>
#include <traffic_light/IMode.idl.h>
#include <traffic_light/ICSTimes.idl.h>

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
uint8_t state_times[TL_MODES_CNT] = {
    0,                                  // если мы в ручно управлении - там и оставаться
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

/* Структура машины состояния */
typedef struct{
    TL_MODES mode;              // код состояния
    uint16_t timer_secs;        // таймер 
    bool entered;               // признак что произошел вход в состояние
} cs_state;

/* Текущее состояние */
static cs_state current_state = {.mode = TL_MODE_UNREG, .timer_secs = 0, .entered=false};

/* структура - описатель кишок IPC для вередачи в функцию IPC */
typedef struct {
    struct IMode_proxy *proxy;
    IMode_FMode_req *req;
    IMode_FMode_res *res;
} TransportDescriptor;

/* инициализатор описателя кишок IPC */
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
    /* Уже вошли в состояние? */
    if (!current_state.entered){
        /*
        *  Еще не вошли в состояние, выполнить действия по входу 
        */
        fprintf(stderr, "[%s] Enter state: %u\n", EntityName, current_state.mode);
        // установить таймер
        current_state.timer_secs = state_times[current_state.mode];

        fprintf(stderr, "[%s] State time: %u\n", EntityName, current_state.timer_secs);

        uint32_t LightsGPIO_code = state_LightsGPIO_codes[current_state.mode];
        // передать режим в LightsGPIO
        //fprintf(stderr, "[%s] Call FMode(0x%08x)...\n", EntityName, LightsGPIO_code);
        LightsGPIO_send(td, 
            LightsGPIO_code
        );

        

        current_state.entered = true;   // флаг "уже вошли"        
    }else{
        /*
        *   Уже в состоянии, обработать таймер
        */

        // если таймер выключен, значит так и остаёмся в режиме
        if (current_state.timer_secs==0) return;

        // истёк ли таймер?
        if (current_state.timer_secs>1){
            current_state.timer_secs--;  // нет, истекаем
        }else{
            // да, перейти в следующее состояние
            current_state.mode = state_transitions[current_state.mode];
            fprintf(stderr, "[%s] Switch to state: %u\n", EntityName, current_state.mode);
            current_state.entered = false;
        }
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

    /*
    *  основной цикл, обслуга машины состояний
    */
    while(true){
        state_control(&td);

        /* крутить цикл будем с интевалом в 1 сек. */
        sleep(1);
    }


    return EXIT_SUCCESS;
}

#include <exception>
#include <iostream>
#include <memory>

#include <kos_net.h>
#include <pthread.h>
#include <strings.h>

#include "general.h"
#include "mqtt-comm.h"
#include <nlohmann/json.hpp>

#include <rtl/string.h>
#define NK_USE_UNQUALIFIED_NAMES

/* Files required for transport initialization. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

#include <traffic_light/Communication.edl.h>
#include <traffic_light/ICSMode.idl.h>

using json = nlohmann::json;

static const char EntityName[] = "Communication";

/* Дескриптор соединения diagcomm_connection (входящее) */
typedef struct {
    NkKosTransport transport;
    ServiceId iid;
    Handle handle;
    Communication_entity_req req;
    char req_buffer[Communication_entity_req_arena_size];
    struct nk_arena req_arena;
    Communication_entity_res res;
    char res_buffer[Communication_entity_res_arena_size];
    struct nk_arena res_arena;
    CDiagComm_component component;
    Communication_entity entity;
} DCode_TransportDescriptor;

static DCode_TransportDescriptor dcode_td;

/* Соединение control_system_connection (исходящее) */
typedef struct{
    struct ICSMode_proxy proxy;
    NkKosTransport transport;
    nk_iid_t riid;
    Handle handle;
    ICSMode_CSMode_req req;
    char req_buffer[ICSMode_CSMode_req_arena_size];
    struct nk_arena req_arena;
    ICSMode_CSMode_res res;
    //char res_buffer[IDiagnostics_DMessage_res_arena_size];
    //struct nk_arena res_arena;
}CSMode_TransportDescriptor;

static CSMode_TransportDescriptor cst_td;

MqqtComm *mqtt;

/* Объявление функции IPC c control_system */
nk_err_t control_system_CSMode(CSMode_TransportDescriptor *CSMode_td, 
                            uint8_t mode, std::array<uint16_t, 8> times, uint16_t manualSet);

const char* GetBrokerAddress()
{
    const char* brokerAddress = getenv("MQTT_BROKER_ADDRESS");
    if (brokerAddress)
    {
        return brokerAddress;
    }
    throw std::runtime_error{"Failed to get MQTT broker address."};
}

int GetBrokerPort()
{
    if (auto brokerPortEnvVariable = getenv("MQTT_BROKER_PORT"))
    {
        try
        {
            return std::stoi(brokerPortEnvVariable);
        }
        catch (const std::logic_error&)
        {}
    }
    throw std::runtime_error{"Failed to get MQTT broker port."};
}

/* Обработчик сообщений темы mode */
int set_mode(const string &message){

    try{
        json jmode = json::parse(message);

        /* 
         * Проверка валидности json
         *
         * поле mode есть всегда
         * 
         * в ручном режиме (mode == 1) должно быть поле manualSet
         * и не должно быть поля times
         * 
         * режим auto может содержать times, но не может manualSet
         * 
         * режим unreg не должен содержать никаких полей.
         */
        if (!jmode.contains("mode")) return -2;
        uint8_t mode = jmode["mode"];

        if ((mode == 1)&&
            ((!jmode.contains("manualSet"))||
            (jmode.contains("times") ) ) ){
                std::cerr << app::AppTag <<
                "Mode [manual] should contain field \"manualSet\" but not \"times\"" 
                << std::endl;
                return -2;
            }

        if ((mode == 2)&&
            (jmode.contains("manualSet") )){
                std::cerr << app::AppTag <<
                "Mode [auto] could'nt contain field \"manualSet\"" 
                << std::endl;
                 return -2;
            }
        
        if ((mode == 3)&&(
            (jmode.contains("manualSet"))||
            (jmode.contains("times") ) ) ){
                std::cerr << app::AppTag <<
                "Mode [ungegulated] could'nt contain fields \"manualSet\" or \"times\"" 
                << std::endl;
                return -2;
            }

        
        
        uint16_t manualSet = 0;
        std::array<uint16_t, 8> times = {0,0,0,0,0,0,0,0};

        switch (mode){
            case 1:
            manualSet = jmode["manualSet"];
            break;
            case 2:
            //std::array<uint8_t, 8> times;
            //times
            if (jmode.contains("times")){
                times=jmode["times"].get<std::array<uint16_t, 8>>();
            }
            break;
        }

        return control_system_CSMode(&cst_td, mode, times, manualSet);


    }catch (json::parse_error& ex){
        std::cerr << app::AppTag <<" json parce error" << std::endl;
        return -1;
    }catch (json::type_error& ex){
        std::cerr << app::AppTag <<" json type error" << ex.what() << std::endl;
        return -1;
    }catch (json::exception& ex){
        std::cerr << app::AppTag <<" json error" << ex.what() << std::endl;
        return -1;
    }
    
    
}

/* обработчик сообщений из подписки */
void OnMqttMessage(const string &topic, const string &message){
    int res;
    std::cerr << app::AppTag <<" Got from topic:" << topic << " msg:" << message << std::endl;

    if (topic == "mode"){
        res = set_mode(message);
        if (res<0){
            mqtt->do_publish("mode_resp", "error");
        }else{
            mqtt->do_publish("mode_resp", "success");
        }
    }
}

#define LIGHTS_OK 42
#define LIGHTS_FORBIDDEN 13
#define LIGHTS_ALLOFF 99
#define LIGHST_DOUBTFUL 63

void dcode_to_mqtt(uint8_t code){
    char msg[128];
    bzero(msg,128);

    char * descript;
    switch(code){
        case 13:
        descript = "forbidden set";
        break;
        case 42:
        descript = "all ok";
        break;
        case 63:
        descript = "doubtful set";
        break;
        case 99:
        descript = "lights off";
        break;
        default:
        descript = "unknown code";
        break;
    }
    sprintf(msg, "%s code[%u]", descript, code);

    mqtt->do_publish("diagnostics", msg);
}

/* Реализация метода  DCode */
static nk_err_t DCode(struct IDiagComm *self,
                        const
                        struct IDiagComm_DCode_req *req,
                        const
                        struct nk_arena *req_arena,
                        struct IDiagComm_DCode_res *res,
                        struct nk_arena *res_arena)
{
    
    //std::cerr << app::AppTag << req->code << std::endl;
    std::cerr << app::AppTag << "got DCode = " << req->code << std::endl;
    dcode_to_mqtt(req->code);
    return NK_EOK;
}

static struct traffic_light_IDiagComm *CreateIDiagCommImpl()
{
    /* Table of implementations of IMode interface methods. */
    static const struct traffic_light_IDiagComm_ops ops = {
        .DCode = DCode
    };

    static struct traffic_light_IDiagComm obj =
    {
        .ops = &ops
    };

    return &obj;
}

/* Инициализатор дескриптора diagcomm_connection (входящее) */
void diagcomm_connection_init(DCode_TransportDescriptor *td){

    /* Get lights gpio IPC handle of "diagcomm_connection". */
    td->handle = ServiceLocatorRegister("diagcomm_connection", NULL, 0, &td->iid);
    assert(td->handle != INVALID_HANDLE);

    /* Initialize transport to control system. */
    NkKosTransport_Init(&td->transport, td->handle, NK_NULL, 0);

    // Структуры запрос/ответ
    struct nk_arena req_arena = NK_ARENA_INITIALIZER(td->req_buffer,
                                        td->req_buffer + sizeof(td->req_buffer));

    
    struct nk_arena res_arena = NK_ARENA_INITIALIZER(td->res_buffer,
                                        td->res_buffer + sizeof(td->res_buffer));

    td->req_arena = req_arena;
    td->res_arena = res_arena;

    // диспетчер компонент
    CDiagComm_component_init(&td->component, CreateIDiagCommImpl());

    /* Initialize lights gpio entity dispatcher. */
    Communication_entity_init(&td->entity, &td->component);
}

/* Процессор  дескриптора lights_gpio_connection (входящее) */
void diagcomm_connection_loop(DCode_TransportDescriptor *td){
    /* Flush request/response buffers. */
    nk_req_reset(&td->req);
    nk_arena_reset(&td->req_arena);
    nk_arena_reset(&td->res_arena);

    /* Wait for request to lights gpio entity. */
    if (nk_transport_recv(&td->transport.base,
                            &td->req.base_,
                            &td->req_arena) != NK_EOK) {
        fprintf(stderr, "[%s] nk_transport_recv error\n", EntityName);
    } else {
        /**
         * Handle received request by calling implementation Mode_impl
         * of the requested Mode interface method.
         */
        Communication_entity_dispatch(&td->entity, &td->req.base_, &td->req_arena,
                                    &td->res.base_, &td->res_arena);
    }

    /* Send response. */
    if (nk_transport_reply(&td->transport.base,
                            &td->res.base_,
                            &td->res_arena) != NK_EOK) {
        fprintf(stderr, "[%s] nk_transport_reply error\n", EntityName);
    }
}

/* Инициализатор дескриптора control_system_connection (исходящее) */
void control_system_connection_init(CSMode_TransportDescriptor *CSMode_td){
    CSMode_td->handle = ServiceLocatorConnect("control_system_connection");
    assert(CSMode_td->handle != INVALID_HANDLE);
    NkKosTransport_Init(&CSMode_td->transport,
                         CSMode_td->handle, 
                         NK_NULL, 0);
    CSMode_td->riid = ServiceLocatorGetRiid(CSMode_td->handle,
                             "controlSystem.csmode");
    assert(CSMode_td->riid  != INVALID_RIID);
    ICSMode_proxy_init(&CSMode_td->proxy,
                                &CSMode_td->transport.base,
                                CSMode_td->riid);
    struct nk_arena arena = NK_ARENA_INITIALIZER(
                                CSMode_td->req_buffer,
                                CSMode_td->req_buffer + 
                                sizeof(CSMode_td->req_buffer));
    CSMode_td->req_arena = arena;
}

nk_err_t control_system_CSMode(CSMode_TransportDescriptor *CSMode_td, 
                        uint8_t mode, std::array<uint16_t, 8> times, uint16_t manualSet){
    nk_err_t rc;

    nk_req_reset(&CSMode_td->req);
    nk_arena_reset(&CSMode_td->req_arena);

    CSMode_td->req.modeReq.mode = mode;
    CSMode_td->req.modeReq.manualSet = manualSet;
    
    nk_uint16_t *reqTimes = nk_arena_alloc(
                    nk_uint16_t,
                    &CSMode_td->req_arena,
                    &CSMode_td->req.modeReq.times,
                    ICSMode_CSMode_req_modeReq_times_size);
    if (reqTimes == RTL_NULL)
    {
        fprintf(
            stderr,
            "[%s]: Error: can`t allocate memory in arena!\n",
            EntityName);

            return NK_ENOMEM;
    }

    rtl_memcpy(reqTimes, times.data(), 16);

    if ((rc = ICSMode_CSMode(&CSMode_td->proxy.base,
                            &CSMode_td->req,
                            &CSMode_td->req_arena,
                            &CSMode_td->res,
                            NULL)
    ) != NK_EOK){
        fprintf(stderr, "[%s] ICSMode_CSMode error %d\n", EntityName, rc);
        return -1;
    }
    return 0;
}

void *diagcomm_loop(void *td){
    while(true){
        diagcomm_connection_loop((DCode_TransportDescriptor *)td);
        sleep(1);
    }
}

int main(void) try
{
    if (!wait_for_network())
    {
        throw std::runtime_error{"Error: Wait for network failed!"};
    }

    mosqpp::lib_init();

    /* создание mqtt объекта */
    //auto mqtt = std::make_unique<MqqtComm>("mqtt-comm", GetBrokerAddress(), GetBrokerPort());
    auto mqtt_obj = MqqtComm("mqtt-comm", GetBrokerAddress(), GetBrokerPort());
    mqtt = &mqtt_obj;
    
    /* подписаться на темы */
    mqtt->subscribe_topic("mode");

    /* Поставить обработчик сообщений */
    mqtt->set_on_message(OnMqttMessage);

    
    /* инициализировать соединение с control system */
    control_system_connection_init(&cst_td);

    /* инициализировать соединение с Diagnostics */
    diagcomm_connection_init(&dcode_td);

    pthread_t thread;
    int retCode = pthread_create(&thread, NULL, diagcomm_loop, &dcode_td);
    if (retCode != 0)
    {
        fprintf(stderr, "[%s] pthread_create returns error %d\n", EntityName, retCode);
        return false;
    }

    /* цикл обслуживания публикатора и подписчика */
    while (true)
    {
        sleep(1);
        
        mqtt->loop();
        
        //diagnostics_connection_loop(&dcode_td);
    }

    mosqpp::lib_cleanup();
    return EXIT_SUCCESS;
}
catch (std::exception& exc)
{
    std::cerr << app::AppTag << exc.what() << std::endl;
    return EXIT_FAILURE;
}

#include <exception>
#include <iostream>
#include <memory>

#include <kos_net.h>

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

/* Инициализатор дескриптора control_system_connection */
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

    

    control_system_connection_init(&cst_td);

    /* цикл обслуживания публикатора и подписчика */
    while (true)
    {
        sleep(1);
        //mqtt->do_publish("datetime", "хрен знает");
        mqtt->loop();
        /* Если есть новые сообщения */
    }

    mosqpp::lib_cleanup();
    return EXIT_SUCCESS;
}
catch (std::exception& exc)
{
    std::cerr << app::AppTag << exc.what() << std::endl;
    return EXIT_FAILURE;
}

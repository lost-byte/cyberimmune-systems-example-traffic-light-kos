#include <exception>
#include <iostream>
#include <memory>

#include <kos_net.h>

#include "general.h"
#include "mqtt-comm.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static const char EntityName[] = "Communication";

MqqtComm *mqtt;

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

        if (!jmode.contains("mode")) return -2;
        return 0;

    }catch (json::parse_error& ex){
        std::cerr << app::AppTag <<" parce error" << std::endl;
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

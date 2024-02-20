#include <exception>
#include <iostream>
#include <memory>

#include <kos_net.h>

#include "general.h"
#include "mqtt-comm.h"


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

void OnMqttMessage(const string &topic, const string &message){
    std::cerr << "Got from topic:" << topic << " msg:" << message << std::endl;
}


int main(void) try
{
    if (!wait_for_network())
    {
        throw std::runtime_error{"Error: Wait for network failed!"};
    }

    mosqpp::lib_init();

    /* создание mqtt объекта */
    auto mqtt = std::make_unique<MqqtComm>("mqtt-comm", GetBrokerAddress(), GetBrokerPort());
    
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

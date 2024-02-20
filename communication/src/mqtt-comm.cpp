#include <iostream>
#include <string_view>

#include <vector>
#include <string>

#include "general.h"
#include "mqtt-comm.h"

using namespace std::literals;
using namespace std;


//constexpr auto Topic = "mode"sv;
vector<string> topics;

MqqtComm::MqqtComm(const char *id, const char *host, int port)
    : mosquittopp(id)
{
    std::cout << app::AppTag << "Connecting to MQTT Broker with address "
              << host << " and port " << port << std::endl;

    const int keepAlive = 60;

    connect(host, port, keepAlive);
}

void MqqtComm::on_connect(int rc)
{
    std::cout << app::AppTag << "Subscriber connected with code "
              << rc << std::endl;

    // if (rc == 0)
    // {
    //     subscribe(NULL, Topic.data());
    // }
    for (vector<string>::iterator it=topics.begin(); it!=topics.end(); ++it){
        subscribe(NULL, it->c_str());
    }

}

void MqqtComm::on_message(const struct mosquitto_message *message)
{
    /*
    if (Topic == message->topic)
    {
        const std::string_view payload{reinterpret_cast<char*>(message->payload),
                                       static_cast<size_t>(message->payloadlen)};
        std::cout << app::AppTag << "Got message with topic: " << message->topic
                  << ", payload: " << payload << std::endl;
    }
    */
    if (on_message_ptr == NULL) return;

    for (vector<string>::iterator it=topics.begin(); it != topics.end(); ++it ){
        const std::string_view payload{reinterpret_cast<char*>(message->payload),
                                       static_cast<size_t>(message->payloadlen)};

        if ( *it == message->topic ){
            string pl(payload);
            on_message_ptr(*it, pl);
        }
    }
}

void MqqtComm::on_subscribe(__rtl_unused int        mid,
                              __rtl_unused int        qos_count,
                              __rtl_unused const int *granted_qos)
{
    std::cout << app::AppTag << "Subscription succeeded." << std::endl;
}

void MqqtComm::on_publish(int)
{
    //std::cout << app::AppTag << "Publication succeeded." << std::endl;
}

void MqqtComm::set_on_message(fn_ptr on_message_fn){
    on_message_ptr = on_message_fn;
}

void MqqtComm::do_publish(const string &topic, const string &message){
    publish(nullptr, topic.c_str(), static_cast<int>(message.size()),
            message.c_str());
}

void MqqtComm::subscribe_topic(string topic){
    topics.push_back(topic);
}
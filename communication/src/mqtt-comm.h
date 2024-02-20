/* © 2021 AO Kaspersky Lab */
#ifndef _MOSQUITTO_SUBSCRIBER_H
#define _MOSQUITTO_SUBSCRIBER_H

#include <string>
#include <mosquittopp.h>
#include <rtl/compiler.h>

using std::string;

typedef void (*fn_ptr)(const string&, const string&); 

class MqqtComm : public mosqpp::mosquittopp
{
public:
    MqqtComm();
    MqqtComm(const char *id, const char *host, int port);
    ~MqqtComm() {};

    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message *message) override;
    void on_subscribe(__rtl_unused int        mid,
                      __rtl_unused int        qos_count,
                      __rtl_unused const int *granted_qos) override;
    void on_publish(int mid) override;

    void set_on_message(fn_ptr on_message_fn_ptr);
    void do_publish(const string &topic, const string &message);
    void subscribe_topic(string topic);

private:
    fn_ptr on_message_ptr = NULL;
};

#endif // _MOSQUITTO_SUBSCRIBER_H

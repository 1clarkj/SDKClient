#include "MQTT_Publisher.h"
#include "MQTTAsync.h"
#include <iostream>
#include <chrono>
#include <thread>

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "AsyncPublisher"
#define TOPIC       "test/topic"
#define QOS         0
#define TIMEOUT     10000L

MQTTAsync client;

void onConnect(void* context, MQTTAsync_successData* response) {
    std::cout << "Connected to broker!" << std::endl;
}

void onSend(void* context, MQTTAsync_successData* response) {
    std::cout << "Message sent!" << std::endl;
}

void publishData(const std::string& payload) {
    MQTTAsync_message message = MQTTAsync_message_initializer;
    message.payload = (void*)payload.c_str();
    message.payloadlen = payload.size();
    message.qos = QOS;
    message.retained = 0;

    int rc = MQTTAsync_sendMessage(client, TOPIC, &message, nullptr);
    if (rc != MQTTASYNC_SUCCESS) {
        std::cout << "Failed to send message, return code: " << rc << std::endl;
    }
}

void initializeMQTTClient() {
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.onSuccess = onConnect;
    conn_opts.context = client;

    MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, nullptr);
    MQTTAsync_setCallbacks(client, nullptr, nullptr, nullptr, nullptr);

    if (MQTTAsync_connect(client, &conn_opts) != MQTTASYNC_SUCCESS) {
        std::cerr << "Failed to start connect to broker!" << std::endl;
    }
}

void disconnectMQTTClient() {
    MQTTAsync_disconnect(client, nullptr);
    MQTTAsync_destroy(&client);
}

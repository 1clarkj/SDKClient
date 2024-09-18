#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include <string>

// Declaration of the publishData function
void publishData(const std::string& payload);

// Declaration of the function to set up and connect the MQTT client
void initializeMQTTClient();

// Declaration of the function to disconnect the MQTT client
void disconnectMQTTClient();

#endif // MQTT_PUBLISHER_H

#pragma once

#define MQTT_URI_BIT (1 << 2)

void mqtt_conf(const char* dyn_mqtt_uri);
void mqtt_publish(char* payload, const char* topic, uint8_t qos);
void bme_publish(void* pv);


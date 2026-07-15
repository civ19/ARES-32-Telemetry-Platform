#pragma once


void mqtt_conf();
void mqtt_publish(char* payload, const char* topic, uint8_t qos);
void bme_publish(void* pv);


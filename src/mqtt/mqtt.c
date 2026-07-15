#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "mqtt_client.h"


#include "abstractions/abstractions.h"
#include "mqtt.h"
#include "sensor/bmetask.h"
#include "json/json.h"

const char *TAGM = "MQTT";
static esp_mqtt_client_handle_t client_handle = NULL;


static void mqtt_event_callback(void *handler_args, esp_event_base_t dept, int32_t event_id, void* event_data) {
    switch(event_id) {
        //connected and disconnected cases
        case MQTT_EVENT_CONNECTED:
            mutexPrint(TAGM, "Mqtt broker connected to ESP32.", 'I');
        break;
        case MQTT_EVENT_DISCONNECTED:
            mutexPrint(TAGM, "Mqtt broker disconnected from ESP32.", 'E');
        break;

        //data cases
        case MQTT_EVENT_DATA: {
            //when data comes in, we take it from event data then show topic and then show message
            esp_mqtt_event_t *payload = (esp_mqtt_event_t *)event_data;
            if(xSemaphoreTake(printMutex, portMAX_DELAY)) {
                ESP_LOGI(TAGM, "Topic: %.*s", payload->topic_len, payload->topic);
                xSemaphoreGive(printMutex);
            }

            if(xSemaphoreTake(printMutex, portMAX_DELAY)) {
                ESP_LOGI(TAGM, "Topic: %.*s", payload->data_len, payload->data);
                xSemaphoreGive(printMutex);
            }

            break;
        }

        case MQTT_EVENT_ERROR: 
            mutexPrint(TAGM, "MQTT Error", 'E');

            esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
            if(event->error_handle != NULL) {
                if(xSemaphoreTake(printMutex, portMAX_DELAY)) {
                    ESP_LOGE(TAGM, "Error type: %d", event->error_handle->error_type);
                    xSemaphoreGive(printMutex);
                }
                if(xSemaphoreTake(printMutex, portMAX_DELAY)) {
                    ESP_LOGE(TAGM, "Error code: %d", event->error_handle->connect_return_code);
                    xSemaphoreGive(printMutex);
                }

            }
        break;
    }
}

void mqtt_conf() {
    //set broker
    esp_mqtt_client_config_t mqtt_config = {};
    mqtt_config.broker.address.uri = "mqtt://10.0.0.74:1883";

    //initializing mqtt client
    client_handle = esp_mqtt_client_init(&mqtt_config);
    //start mqtt
    ESP_ERROR_CHECK(esp_mqtt_client_start(client_handle)); //starts the connection process from esp to mqtt broker
    mutexPrint(TAGM, "Mqtt Started.", 'I');
    //add instance register

    ESP_ERROR_CHECK(esp_mqtt_client_register_event(client_handle, (esp_mqtt_event_id_t) ESP_EVENT_ANY_ID, mqtt_event_callback, NULL));

}

void mqtt_publish(char* payload, const char* topic, uint8_t qos) {
    if(client_handle == NULL)
    {
        mutexPrint(TAGM, "Mqtt not init", 'E');
        return;
    }

    else esp_mqtt_client_publish(client_handle, topic, payload, strlen(payload), qos, 0);
}

void bme_publish(void *pv) {
    bme_payload_t data;

    for(;;) {
        if(xQueueReceive(bme_queue, &data, portMAX_DELAY)) mutexPrint("BME_QUEUE", "Data received. Sending to mqtt...", 'I');
        send_payload(data.temp, data.hum, data.pres);
    }
    
}
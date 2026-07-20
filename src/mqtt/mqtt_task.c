#include "mqtt_task.h"
#include "nimble_prov/ble_gatt.h"
#include "mqtt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

#include "abstractions/abstractions.h"

static char dyn_mqtt_uri[MAX_MQTT_LEN + 1];

TaskHandle_t mqtt_uri_handle = NULL;

void mqtt_prov_task(void *pv) {
    for(;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(500));

        mutexPrint("MQTT", "Mqtt broker uri received.", 'I');
        
        mqtt_conf(dyn_mqtt_uri);
    }

}

void trigger_mqtt_prov(const char* uri) {
    strlcpy((char*) dyn_mqtt_uri, uri,  sizeof(dyn_mqtt_uri));

    if(mqtt_uri_handle != NULL) xTaskNotifyGive(mqtt_uri_handle);

}



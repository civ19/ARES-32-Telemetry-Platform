#include "cJSON.h"
#include "mqtt_client.h"
#include <stdlib.h>
#include "freertos/FreeRTOS.h"

#include "mqtt/mqtt.h"
#include "json.h"


void send_payload(float t, float h, float p) {

        cJSON *root = cJSON_CreateObject();

        //build the json and add stuff to it
        cJSON_AddNumberToObject(root, "temperature", t);
        cJSON_AddNumberToObject(root, "humidity", h);
        cJSON_AddNumberToObject(root, "pressure", p);

        //stringify the root
        char* json_str = cJSON_Print(root);
        mqtt_publish(json_str, "esp32/data", 0);
        free(json_str);
        cJSON_Delete(root);
    
}

#include "cJSON.h"
#include "mqtt_client.h"
#include <stdlib.h>

#include "mqtt/mqtt.h"

void send_payload() {
    cJSON *root = cJSON_CreateObject();

    //build the json and add stuff to it
    cJSON_AddNumberToObject(root, "temperature", 24.5);
    cJSON_AddNumberToObject(root, "humidity", 51.2);
    cJSON_AddNumberToObject(root, "pressure", 1013.4);

    //stringify the root
    char* json_str = cJSON_Print(root);
    mqtt_publish(json_str, "esp32/data", 0);
    free(json_str);
    cJSON_Delete(root);
    
}
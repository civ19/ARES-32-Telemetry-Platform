#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "bme280.h"

i2c_master_bus_handle_t bme_handle;
extern QueueHandle_t bme_queue;

typedef struct {
    float temp;
    float hum;
    float pres;
} bme_payload_t;

extern bme_payload_t data;

void bme_read_task(void *pv);


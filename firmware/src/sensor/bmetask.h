#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/i2c_master.h"


extern i2c_master_dev_handle_t bme_handle;
extern QueueHandle_t bme_queue;

typedef struct {
    float temp;
    float hum;
    float pres;
} bme_payload_t;



void bme_read_task(void *pv);


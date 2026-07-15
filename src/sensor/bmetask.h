#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "bme280.h"

i2c_master_bus_handle_t bme_handle;
extern QueueHandle_t bme_queue;

void bme_read_task(void *pv);


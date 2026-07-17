#include "ble_master.h"
#include "esp_log.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "ble_gatt.h"
#include "ble_gap.h"
#include "abstractions/abstractions.h"

#define TAGB "BLE_INIT"

static uint8_t own_addr_type = BLE_OWN_ADDR_PUBLIC;

//sync cb
static void ble_app_on_sync(void) {
    int rc = ble_hs_id_infer_auto(0, &own_addr_type);
    if(rc != 0) {
        mutex_log(TAGB, "Fatal: Failed to infer ble address type. rc=%d", 'E', rc);
        return;
    }

    mutexPrint(TAGB, "Hardware sync complete. Addr type verified.", 'I');
    ble_app_advertise();

}

//host task
void ble_host_task(void *param) {
    mutex_log(TAGB, "BLE host worker loop launched on Core %d", 'I', xPortGetCoreID());
    nimble_port_run();

    mutexPrint(TAGB, "NimBLE event loop exited. Destroyed thread context...", 'W');
    vTaskDelete(NULL);
}


esp_err_t init_ble_provisioning(void) {

    int rc = nimble_port_init();
    if(rc!=0) {
        mutex_log(TAGB, "Init failed! Memory alloc arror: rc=%d", 'E', rc);
        return ESP_FAIL;
    }

    ble_hs_cfg.sync_cb = ble_app_on_sync; //attaching synbc cb 

    ble_svc_gap_init();
    ble_svc_gatt_init();

    //profile structure init and validation
    rc = ble_gatts_count_cfg(gatt_svr_svcs); //allocating the right memory for tghe gatt table
    if(rc!=0) {
        mutex_log(TAGB, "GATT Table Memory alloc arror: rc=%d", 'E', rc);
        return ESP_FAIL;
    }

    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    if(rc!=0) {
        mutex_log(TAGB, "Failed to add GATT table to BT Database pool: rc=%d", 'E', rc);
        return ESP_FAIL;
    }

    //name for dev
    rc = ble_svc_gap_device_name_set("S3-WEATHER-STATION");
    if(rc!=0) {
        mutex_log(TAGB, "Failed to apply public gap node name: rc=%d", 'E', rc);
        return ESP_FAIL;
    }

    //task on core 0
    mutexPrint(TAGB, "Starting async BT task...", 'I');
    nimble_port_freertos_init(ble_host_task);

    return ESP_OK;


}
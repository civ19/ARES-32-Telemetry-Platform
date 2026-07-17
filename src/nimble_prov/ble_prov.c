#include "host/ble_hs.h"
#include "services/gatt/ble_svc_gatt.h"
#include <string.h>

#include "abstractions/abstractions.h"
#include "esp_log.h"
#include "ble_prov.h"

static char wifi_ssid[MAX_SSID_LEN + 1]; //ssid and pass containers
static char wifi_pass[MAX_PASS_LEN + 1];

const char *TAG = "BLE_SVR";

static int gatt_svr_access_cb(uint16_t conn_handle, uint16_t attr_handle, 
    struct ble_gatt_access_ctxt* ctx, void* arg);




#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"







idf_component_register(
    SRCS
        "main.c"
        "mqtt/mqtt.c"
        "wifi/wifi.c"
        "wifi/wifi_task.c"
        "abstractions/abstractions.c"
        "json/json.c"
        "bme_driver/bme280.c"
        "bme_driver/bmetask.c"
        "nimble_prov/ble_gap.c"
        "nimble_prov/ble_gatt.c"
        "nimble_prov/ble_master.c"
        "mqtt/mqtt_task.c"

    INCLUDE_DIRS
        "."
        "nimble_prov"
        "bme_driver"

    REQUIRES
        driver
        esp_adc
        mqtt
        cjson
        bt
)
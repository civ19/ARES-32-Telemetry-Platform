idf_component_register(
    SRCS
        "main.c"
        "mqtt/mqtt.c"
        "wifi/wifi.c"
        "abstractions/abstractions.c"
        "json/json.c"
        "sensor/bme280.c"
        "sensor/bmetask.c"
        "nimBLE/ble_prov.c"

    INCLUDE_DIRS
        "."
        "nimBLE"

    REQUIRES
        driver
        esp_adc
        mqtt
        cjson
        bt
)
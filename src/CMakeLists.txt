idf_component_register(
    SRCS
        "main.cpp"
        "mqtt/mqtt.cpp"
        "wifi/wifi.cpp"

    INCLUDE_DIRS
        "."

    REQUIRES
        driver
        esp_adc
        mqtt
        cjson
)
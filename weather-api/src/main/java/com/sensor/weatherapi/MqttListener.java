package com.sensor.weatherapi;

import lombok.RequiredArgsConstructor;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.stereotype.Component;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.extern.slf4j.Slf4j;

@Component
@RequiredArgsConstructor
@Slf4j
public class MqttListener implements AutoCloseable {

    private final SensorService service;
    private final ObjectMapper objectMapper;
    private MqttClient client;

    public void start() throws MqttException {
        //connect toclient
        this.client = new MqttClient("tcp://10.0.0.74:1883", MqttClient.generateClientId());
        this.client.connect();

        this.client.subscribe("esp32/data", (topic, msg) -> {
            String payload = new String(msg.getPayload()); //getting my payloasd
            try {
                SensorResponse resp = objectMapper.readValue(payload, SensorResponse.class);
                service.saveData(resp);
            } catch (Exception e) {
                log.error("Failed to parse or save ESP32 sensor payload: {}", payload, e);
            }
        });
    }

    @Override
    public void close() throws MqttException {
        //close when class obj is destoryed
        if(client != null && client.isConnected()) {
            client.disconnect();
            client.close();
        }

    }


}
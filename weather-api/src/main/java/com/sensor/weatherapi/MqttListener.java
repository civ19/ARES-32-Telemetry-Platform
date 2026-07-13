package com.sensor.weatherapi;

import lombok.RequiredArgsConstructor;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;

import org.springframework.stereotype.Component;

import com.fasterxml.jackson.databind.ObjectMapper;

@Component
@RequiredArgsConstructor
public class MqttListener {

    private final SensorService service;
    private final ObjectMapper objectMapper;

    public void start() throws MqttException {
        //connect toclient
        MqttClient client = new MqttClient("tcp://10.0.0.74:1883", MqttClient.generateClientId());
        client.connect();
        client.subscribe("esp32/data", (topic, msg) -> {
            String payload = new String(msg.getPayload()); //getting my payloasd
            try {
                objectMapper.readValue(payload, SensorResponse.class);
                service.saveData()
            }
        });

    }


}
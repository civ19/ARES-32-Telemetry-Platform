package com.sensor.weatherapi;

import jakarta.annotation.PostConstruct;
import jakarta.annotation.PreDestroy;
import lombok.RequiredArgsConstructor;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.stereotype.Component;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.extern.slf4j.Slf4j;

@Component
@RequiredArgsConstructor
@Slf4j
public class MqttListener {

    private final SensorService service;
    private final ObjectMapper objectMapper;
    private MqttClient client;

    public void mqttConnect() throws MqttException{
        log.info("Connecting to Mqtt Broker");
        client = new MqttClient("tcp://10.0.0.74:1883", MqttClient.generateClientId());
        client.connect();
    }

    @PostConstruct
    public void start() throws MqttException {
        //connect toclient
        try {
            mqttConnect();
            log.info("Mqtt Connected successfully! Subscribing to esp32/data..");
            client.subscribe("esp32/data", (topic, msg) -> {
                String payload = new String(msg.getPayload()); //getting my payloasd
                try {
                    SensorResponse resp = objectMapper.readValue(payload, SensorResponse.class);
                    service.saveData(resp);
                } catch (Exception e) {
                    log.error("Failed to parse or save ESP32 sensor payload: {}", payload, e);
                }
            });

        } catch (Exception e) {
            log.error("Failed to initialize MQTT Connection", e);
        }
    }


    @PreDestroy
    public void stop() {
        if(client != null) {
            try {
                if(client.isConnected()) {
                    client.disconnect();
                    log.info("MQTT Disconnected.");
                }
                client.close();
            } catch(MqttException e) {log.error("Error during mqtt shutdown");}
        }
    }


}
package com.sensor.weatherapi;

import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

@Service
@RequiredArgsConstructor
public class SensorService {
    private final SensorRepository repo;

    public SensorResponse saveData(SensorResponse resp) {
        Sensor sensor = new Sensor();
        //confiug
        sensor.setId(resp.id());
        sensor.setTemp(resp.temp());
        sensor.setPressure(resp.pressure());


    }

}
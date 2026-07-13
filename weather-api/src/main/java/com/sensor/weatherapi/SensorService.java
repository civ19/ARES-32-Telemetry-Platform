package com.sensor.weatherapi;

import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

@Service
@RequiredArgsConstructor
public class SensorService {
    private final SensorRepository repo;

    public SensorResponse saveData() {
        Sensor sensor = new Sensor()
    }

}
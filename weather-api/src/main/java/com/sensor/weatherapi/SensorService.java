package com.sensor.weatherapi;

import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@Service
@RequiredArgsConstructor
public class SensorService {
    private final SensorRepository repo;

    public SensorResponse saveData(SensorResponse resp) {
        Sensor sensor = Sensor.builder()
                .temp(resp.temp()).humidity(resp.humidity()).pressure(resp.pressure()).build();

        Sensor updated = repo.save(sensor);
        return new SensorResponse(updated.getId(), updated.getTemp(), updated.getHumidity(), updated.getPressure());
    }

    public List<SensorResponse> getAll24() {
        return repo.findAll().stream().map(sensor -> new SensorResponse(sensor.getId(), sensor.getTemp(), sensor.getHumidity(), sensor.getPressure()))
                .collect(Collectors.toList());
    }

    public SensorResponse getNow() {

    }

}
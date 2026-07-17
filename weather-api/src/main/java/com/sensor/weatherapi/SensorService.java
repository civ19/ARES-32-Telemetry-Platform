package com.sensor.weatherapi;

import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.stream.Collectors;

@Service
@RequiredArgsConstructor
public class SensorService {
    private final SensorRepository repo;

    public SensorResponse saveData(SensorResponse resp) {
        Sensor sensor = Sensor.builder()
                .temp(resp.temperature())
                .humidity(resp.humidity())
                .pressure(resp.pressure())
                .build();

        Sensor updated = repo.save(sensor);
        // USE 'updated', NOT 'sensor' for the timestamp
        return new SensorResponse(updated.getId(), updated.getTemp(), updated.getHumidity(), updated.getPressure(), updated.getTimestamp());
    }

    public List<SensorResponse> getAll24() {
        return repo.findAll().stream().map(sensor -> new SensorResponse(
                sensor.getId(),
                sensor.getTemp(),
                sensor.getHumidity(),
                sensor.getPressure(),
                sensor.getTimestamp() // Ensure this is not null in DB
        )).collect(Collectors.toList());
    }

    public SensorResponse getNow() {
        Sensor s = repo.findTopByOrderByTimestampDesc().orElseThrow(() -> new ReadingNotFoundException("Reading not found."));
        return new SensorResponse(s.getId(), s.getTemp(), s.getHumidity(), s.getPressure(), s.getTimestamp());
    }

}
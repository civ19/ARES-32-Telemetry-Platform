package com.sensor.weatherapi;

import lombok.RequiredArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.http.ResponseEntity;

import java.util.List;

@RestController
@RequiredArgsConstructor
public class SensorController {

    private final SensorService service;

    @GetMapping("/api/sensors")
    public ResponseEntity<List<SensorResponse>> getReadings() {
        List<SensorResponse> resp = service.getAll24();
        return new ResponseEntity<List<SensorResponse>>(resp, HttpStatus.OK);
    }
}
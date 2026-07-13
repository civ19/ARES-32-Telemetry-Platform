package com.sensor.weatherapi;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.http.ResponseEntity;

@RestController
public class SensorController {

    @GetMapping("/weather/data")
    public ResponseEntity<SensorResponse> getReadings
}
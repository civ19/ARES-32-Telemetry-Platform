package com.sensor.telemetryapi;

import jakarta.persistence.Table;
import jdk.jfr.Name;
import lombok.RequiredArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.http.ResponseEntity;

import java.util.List;

@CrossOrigin(origins = "*")
@RestController
@RequiredArgsConstructor
public class SensorController {

    private final SensorService service;

    @GetMapping("/api/sensors")
    public ResponseEntity<List<SensorResponse>> getReadings24(@RequestParam(required = false) String range) {
        List<SensorResponse> resp = service.getAll24();
        return new ResponseEntity<>(resp, HttpStatus.OK);
    }

    @GetMapping("/api/sensors/now")
    public ResponseEntity<SensorResponse> getCurrent() {
        SensorResponse resp = service.getNow();
        return new ResponseEntity<>(resp, HttpStatus.OK);
    }
}
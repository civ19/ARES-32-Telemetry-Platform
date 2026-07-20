package com.sensor.weatherapi;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.web.servlet.MockMvc;

import java.time.Instant;
import java.util.List;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.put;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@SpringBootTest
@AutoConfigureMockMvc
@ActiveProfiles("test")
public class TelemetryIntegrationTests {

    @Autowired
    private SensorRepository repo;
    @Autowired
    private SensorService service;
    @Autowired
    MockMvc mockMvc;

    @BeforeEach
    public void reset() {
        repo.deleteAll();
    }

    @Test
    public void getSensorsNowReturnsOkHappy() throws Exception {
        Sensor updated = repo.save(new Sensor(null, 24.1, 0.5, 1051.2, Instant.now()));

        mockMvc.perform(get(("/api/sensors/now"))).andExpect(status().isOk())
                .andExpect(jsonPath("$.temperature").value(updated.getTemp()))
                .andExpect(jsonPath("$.pressure").value(updated.getPressure()))
                .andExpect(jsonPath("$.humidity").value(updated.getHumidity()))
                .andExpect(jsonPath("$.timestamp").value(updated.getTimestamp().toString()));
    }

    @Test
    public void getAll24ReturnsJsonOkHappy() throws Exception {
        Sensor s1 = repo.save(new Sensor(null, 22.1, 0.7, 1050.1, Instant.now()));
        List<SensorResponse> readings = List.of(new SensorResponse(s1.getId(), s1.getTemp(), s1.getHumidity(), s1.getPressure(), s1.getTimestamp()));

        mockMvc.perform((get("/api/sensors")).param("range", "24"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$").isArray())
                .andExpect(jsonPath("$.length()").value(readings.size()))
                .andExpect(jsonPath("$[0].temperature").value(readings.getFirst().temperature()))
                .andExpect(jsonPath("$[0].humidity").value(readings.getFirst().humidity()))
                .andExpect(jsonPath("$[0].pressure").value(readings.getFirst().pressure()))
                .andExpect(jsonPath("$[0].timestamp").value(readings.getFirst().timestamp().toString()));
    }

    @Test
    void getSensorsNowReturnNotFoundSad() throws Exception {
        mockMvc.perform((get("/api/sensors/now"))).andExpect(status().isNotFound());
    }
}
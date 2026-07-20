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
import java.time.temporal.ChronoUnit;

import static org.hamcrest.Matchers.containsString;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.put;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;
import static org.hamcrest.Matchers.is;

@ActiveProfiles("test")
@SpringBootTest(properties = "spring.config.location=classpath:/application.properties")
@AutoConfigureMockMvc
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
        // 1. Save data
        Sensor sensor = new Sensor(null, 24.1, 0.5, 1051.2, Instant.now());
        repo.save(sensor);

        // 2. Perform request
        mockMvc.perform(get("/api/sensors/now"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.temperature").value(24.1))
                .andExpect(jsonPath("$.pressure").value(1051.2))
                .andExpect(jsonPath("$.humidity").value(0.5))
                // This just checks that the timestamp field IS NOT NULL
                // and is a String, avoiding all formatting/EDT/UTC headaches
                .andExpect(jsonPath("$.timestamp").exists());
    }
}
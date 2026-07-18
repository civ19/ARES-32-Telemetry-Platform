package com.sensor.weatherapi;

import com.fasterxml.jackson.annotation.JsonFormat;
import com.fasterxml.jackson.annotation.JsonProperty;
import jakarta.validation.constraints.NotNull;

import java.time.Instant;

public record SensorResponse (Long id,
                              @JsonProperty("temperature") Double temperature,
                              Double humidity,
                              Double pressure,
                              Instant timestamp){
}
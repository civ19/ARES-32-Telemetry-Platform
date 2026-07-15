package com.sensor.weatherapi;

import com.fasterxml.jackson.annotation.JsonProperty;
import jakarta.validation.constraints.NotNull;

public record SensorResponse (@NotNull Long id,
                              @NotNull  @JsonProperty("temperature") Double temp,
                              @NotNull Double humidity,
                              @NotNull Double pressure){
}
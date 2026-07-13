package com.sensor.weatherapi;

import jakarta.validation.constraints.NotNull;

public record SensorResponse (@NotNull Long id, @NotNull Double temp, @NotNull Double humidity, @NotNull Double pressure){
}
package com.sensor.weatherapi;

public class ReadingNotFoundException extends RuntimeException {
    public ReadingNotFoundException(String message) {
        super(message);
    }
}

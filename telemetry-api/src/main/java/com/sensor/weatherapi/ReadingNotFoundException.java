package com.sensor.weatherapi;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(value = HttpStatus.NOT_FOUND, reason = "Reading not found.")
public class ReadingNotFoundException extends RuntimeException {
    public ReadingNotFoundException(String message) {
        super(message);
    }
}

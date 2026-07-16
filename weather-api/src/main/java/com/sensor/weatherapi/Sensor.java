package com.sensor.weatherapi;

import com.fasterxml.jackson.annotation.JsonProperty;
import jakarta.persistence.*;
import lombok.*;
import org.hibernate.annotations.CreationTimestamp;

import java.time.Instant;
import java.time.LocalDateTime;

@Entity
@RequiredArgsConstructor
@AllArgsConstructor
@Getter
@Setter
@Builder
public class Sensor {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    Long id;

    @Column(nullable = false)
    private Double temp;
    @Column(nullable = false)
    private Double humidity;
    @Column(nullable = false)
    private Double pressure;

    @CreationTimestamp
    @Column(updatable = false, nullable = false)
    private Instant timestamp;
}
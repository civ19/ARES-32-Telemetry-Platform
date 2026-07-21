package com.sensor.telemetryapi;

import com.fasterxml.jackson.annotation.JsonProperty;
import jakarta.persistence.*;
import lombok.*;
import org.hibernate.annotations.CreationTimestamp;

import java.time.Instant;
import java.time.LocalDateTime;

@Entity
@NoArgsConstructor
@AllArgsConstructor
@Getter
@Setter
@Table(name = "sensor")
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
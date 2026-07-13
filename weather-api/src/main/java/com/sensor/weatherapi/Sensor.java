package com.sensor.weatherapi;

import jakarta.persistence.*;
import lombok.*;

@Entity
@RequiredArgsConstructor
@AllArgsConstructor
@NoArgsConstructor
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
}
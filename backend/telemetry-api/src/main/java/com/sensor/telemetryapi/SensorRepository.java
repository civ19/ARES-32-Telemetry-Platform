package com.sensor.telemetryapi;

import org.springframework.data.jpa.repository.JpaRepository;
import java.util.Optional;

public interface SensorRepository extends JpaRepository<Sensor, Long> {
    Optional<Sensor> findTopByOrderByTimestampDesc();
}
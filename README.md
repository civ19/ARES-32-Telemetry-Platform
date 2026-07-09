# Weather Station 🌦️

An IoT-based weather monitoring system that collects environmental data from a BME280 sensor, publishes readings over MQTT, stores historical data, and displays live conditions through a web dashboard.

## Features

* 🌡️ **Environmental Monitoring**

  * Temperature readings
  * Humidity measurements
  * Atmospheric pressure tracking
  * Data collected using the BME280 sensor

* 📡 **IoT Communication**

  * ESP32-based sensor node
  * WiFi connectivity using ESP-IDF
  * MQTT messaging architecture
  * Mosquitto MQTT broker for communication

* 🖥️ **Web Dashboard**

  * Live weather data visualization
  * Browser-based interface
  * Historical graphs displaying the previous 24 hours of readings

* ⚙️ **Backend System**

  * Java Spring Boot REST API
  * Handles communication between database and frontend
  * Processes incoming sensor data

* 🗄️ **Database Storage**

  * PostgreSQL database
  * Stores historical weather measurements
  * Enables long-term data tracking and visualization

## System Architecture

```
BME280 Sensor
      |
      v
ESP32 (ESP-IDF)
      |
      | MQTT
      v
Mosquitto Broker
      |
      v
Spring Boot Backend
      |
      v
PostgreSQL Database
      |
      v
HTML / CSS / JavaScript Dashboard
```

## Technologies Used

### Embedded

* ESP32
* ESP-IDF
* C/C++
* BME280 sensor
* WiFi
* MQTT

### Backend

* Java
* Spring Boot
* REST API
* PostgreSQL

### Frontend

* HTML
* CSS
* JavaScript
* Data visualization graphs

### Networking

* MQTT
* Mosquitto Broker

## Project Goals

This project demonstrates a full-stack IoT pipeline:

1. Collect sensor data from physical hardware
2. Transmit readings wirelessly using MQTT
3. Process and expose data through a backend API
4. Store historical measurements
5. Visualize environmental trends through a web dashboard

## Future Improvements

* Add additional sensors (air quality, light, CO₂, etc.)
* Deploy backend/database services to the cloud
* Add authentication and user accounts
* Add mobile-friendly dashboard support
* Implement alerts for abnormal readings

## Author

Built as a full-stack IoT learning project combining embedded systems, networking, backend development, databases, and frontend visualization.

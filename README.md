# ARES-32: Distributed IoT Telemetry Engine & AI-Integration Bridge

[![Platform: ESP-IDF](https://img.shields.io/badge/Platform-ESP--IDF-red)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
[![RTOS: FreeRTOS](https://img.shields.io/badge/RTOS-FreeRTOS-blue)](https://www.freertos.org/)
[![Backend: Spring Boot](https://img.shields.io/badge/Backend-Spring--Boot-green)](https://spring.io/projects/spring-boot)
[![Infrastructure: Docker](https://img.shields.io/badge/Infrastructure-Docker-blue)](https://www.docker.com/)

**ARES-32** (Asynchronous Remote Environmental Sensing) is a distributed telemetry platform designed for high-concurrency data acquisition and AI-agentic integration. This project features a full-vertical stack: a custom C driver for Bosch sensors, a multi-core FreeRTOS firmware architecture, and a containerized Spring Boot backend exposed to AI models via the Model Context Protocol (MCP).

<img width="2536" height="1243" alt="image" src="https://github.com/user-attachments/assets/f1a55be7-b1ad-43e8-99b9-27a9196aa05b" />

---

## System Architecture

The platform is architected as a decoupled, multi-service environment:

1.  **Firmware Layer (`/firmware`):** C-based ESP-IDF application managing sensor acquisition and NimBLE provisioning.
2.  **Backend Layer (`/backend`):** Java Spring Boot microservice providing RESTful endpoints, PostgreSQL persistence, and MQTT data ingestion.
3.  **Intelligence Bridge (`/gateway`):** Python-based FastMCP server that exposes real-time hardware telemetry to AI Agents (Claude/GPT-4).
4.  **Orchestration (`/`):** Unified Docker-Compose environment for reproducible, sandbox-ready deployment.

---

## Technical Features

### 1. Embedded Systems & Concurrency
*   **Custom Sensor Driver:** Developed a C-based I2C driver for the Bosch BME280, implementing manual register mapping and factory calibration coefficient compensation logic directly from the datasheet.
*   **FreeRTOS Task Management:** Leveraged a multi-threaded architecture using **Task Notifications** for event-driven synchronization and **Queues** for thread-safe IPC between acquisition and network tasks.
*   **Resource Protection:** Implemented **Mutex-guarded hardware abstraction** for thread-safe logging and UART access across dual-core execution environments.

### 2. Network & Industrial Protocols
*   **MQTT Pipeline:** Architected a robust telemetry pipeline using **MQTT and cJSON** for structured data serialization, enabling low-latency communication between edge nodes and the backend.
*   **NimBLE Provisioning:** Developed a custom **BLE GATT Profile** for secure, out-of-band WiFi credential handoff and MQTT broker URI configuration.
*   **Event-Driven Connectivity:** Utilized **FreeRTOS Event Groups** to manage complex network states (WiFi/MQTT) and automated reconnection logic.

<img width="1914" height="980" alt="image" src="https://github.com/user-attachments/assets/8883707c-450c-46d3-91d1-1d7a82a7d98a" />

### 3. AI-Agentic Integration (The "Bridge")
*   **Model Context Protocol (MCP):** Implementation of the **FastMCP** framework to bridge hardware-level telemetry with AI sandboxes.
*   **Semantic Resources:** Exposes `telemetry://current` and `telemetry://history` as readable primitives for LLM trend analysis.
*   **Dockerized Infrastructure:** Full containerization of the backend and gateway services to ensure stability and reproducibility in isolated sandbox environments.

---

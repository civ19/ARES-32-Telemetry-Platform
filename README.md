# ARES-32: Distributed IoT Telemetry & Fault-Tolerant Reliability Platform

[![Platform: ESP-IDF](https://img.shields.io/badge/Platform-ESP--IDF-red)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
[![RTOS: FreeRTOS](https://img.shields.io/badge/RTOS-FreeRTOS-blue)](https://www.freertos.org/)
[![Backend: Spring Boot](https://img.shields.io/badge/Backend-Spring--Boot-green)](https://spring.io/projects/spring-boot)
[![Infrastructure: Docker](https://img.shields.io/badge/Infrastructure-Docker-blue)](https://www.docker.com/)

**ARES-32** (Asynchronous Remote Environmental Sensing) is a mission-critical IoT ecosystem designed for high-availability telemetry and autonomous field recovery. This project demonstrates a full-vertical integration: from bare-metal C drivers and FreeRTOS task orchestration to containerized Java backends and AI-Agentic interfaces via the Model Context Protocol (MCP).

---

## System Architecture

The platform is architected as a decoupled multi-service environment:

1.  **Firmware Layer (`/firmware`):** C-based ESP-IDF application managing raw I2C sensor data, NimBLE provisioning, and FreeRTOS concurrency.
2.  **Backend Layer (`/backend`):** Java Spring Boot microservice providing RESTful endpoints, PostgreSQL persistence, and MQTT ingestion.
3.  **Intelligence Bridge (`/gateway`):** Python-based FastMCP server that exposes real-time hardware telemetry and system tools to AI Agents (Claude/GPT-4).
4.  **Orchestration (`/`):** Unified Docker-Compose environment for reproducible, sandbox-ready deployment.

---

## Technical Features

### 1. Embedded Systems & RTOS (The "Metal")
*   **Bare-Metal Drivers:** Custom I2C implementation for the Bosch BME280, featuring manual register mapping and factory calibration coefficient compensation logic.
*   **Concurrency & IPC:** Multithreaded architecture using **FreeRTOS Queues** for data passing and **Mutexes** for thread-safe hardware abstraction (HAL) and logging.
*   **Provisioning:** Custom **NimBLE (BLE) GATT Profile** for secure, out-of-band WiFi credential handoff and MQTT broker configuration.

### 2. Sentinel-OS: Fault-Tolerance (The "Reliability")
*   **Post-Mortem Debugging:** Automated **Core Dump** capture to dedicated Flash partitions upon system panic or Watchdog timeout.
*   **Self-Healing Logic:** Multi-stage **Watchdog Timers (WDT)** and `esp_reset_reason()` analysis to ensure autonomous recovery from deadlocks or memory corruption.
*   **HTTPS OTA:** Secure Over-the-Air updates with rollback protection and version validation.

### 3. AI-Agentic Integration (The "Bridge")
*   **Model Context Protocol (MCP):** Implementation of the **FastMCP** framework to bridge low-level telemetry with AI sandboxes.
*   **Semantic Resources:** Exposes `telemetry://current` and `telemetry://history` as readable primitives for LLM trend analysis.
*   **Remote Tools:** Agentic capabilities for `verify_system_integrity()` and `trigger_remote_reboot()`.

---

## Quick Start (Production Sandbox)

The entire backend infrastructure is containerized for "One-Command" deployment:

```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/ARES-32-Platform.git
cd ARES-32-Platform

# Launch the integrated stack (Postgres, Spring Boot, MCP Gateway)
docker-compose up --build

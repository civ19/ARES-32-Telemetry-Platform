/**
 * Weather Station - System Architect Frontend
 * Handles real-time telemetry updates and state management.
 */

const CONFIG = {
    API_BASE_URL: 'http://localhost:8080/api', // Spring Boot endpoint
    WS_ENDPOINT: 'ws://localhost:8080/ws',    // WebSocket endpoint
    OFFLINE_TIMEOUT: 15000                   // 15s without data = Offline
};

// Global State
let lastUpdateReceived = null;
let statusCheckInterval = null;

// DOM Elements
const elements = {
    temp: document.getElementById('temp-val'),
    hum: document.getElementById('hum-val'),
    pres: document.getElementById('pres-val'),
    statusDot: document.getElementById('status-dot'),
    statusText: document.getElementById('status-text'),
    lastUpdateText: document.getElementById('update-time'),
    buttons: document.querySelectorAll('.time-buttons button')
};

/**
 * 1. UI UPDATERS
 * High-level functions to transform raw data into a polished UI.
 */
const updateTelemetryUI = (data) => {
    elements.temp.innerText = `${data.temperature.toFixed(1)}°C`;
    elements.hum.innerText  = `${data.humidity.toFixed(1)}%`;
    elements.pres.innerText = `${data.pressure.toFixed(0)} hPa`;
    
    lastUpdateReceived = new Date();
    updateStatus(true);
    renderTimestamp();
};

const updateStatus = (isOnline) => {
    if (isOnline) {
        elements.statusDot.style.backgroundColor = '#238636'; // GitHub Green
        elements.statusText.innerText = 'System Online';
    } else {
        elements.statusDot.style.backgroundColor = '#f85149'; // GitHub Red
        elements.statusText.innerText = 'System Offline';
    }
};

const renderTimestamp = () => {
    if (!lastUpdateReceived) return;
    elements.lastUpdateText.innerText = `Last updated: ${lastUpdateReceived.toLocaleTimeString()}`;
};

/**
 * 2. EVENT HANDLERS
 * Managing the "Time Range" button state.
 */
elements.buttons.forEach(btn => {
    btn.addEventListener('click', () => {
        elements.buttons.forEach(b => b.classList.remove('active'));
        btn.classList.add('active');
        
        const range = btn.getAttribute('data-range');
        console.log(`Architect: Requesting data for range: ${range}`);
        // TODO: Trigger Chart.js refresh for this range
    });
});

/**
 * 3. CONNECTION WATCHDOG
 * If the ESP32 stops sending data, we want the UI to reflect it.
 */
const startWatchdog = () => {
    statusCheckInterval = setInterval(() => {
        if (lastUpdateReceived && (new Date() - lastUpdateReceived > CONFIG.OFFLINE_TIMEOUT)) {
            updateStatus(false);
        }
    }, 5000);
};

/**
 * 4. INITIALIZATION
 */
const init = async () => {
    console.info("Architect: Initializing Telemetry Dashboard...");
    
    // Fetch latest reading on load
    try {
        const response = await fetch(`${CONFIG.API_BASE_URL}"/api/sensors"`);
        const data = await response.json();
        updateTelemetryUI(data);
    } catch (err) {
        console.error("Architect: Failed to fetch initial data", err);
        updateStatus(false);
    }

    startWatchdog();
};

document.addEventListener('DOMContentLoaded', init);
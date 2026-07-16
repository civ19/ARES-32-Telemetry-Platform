import { renderWeatherChart } from './charts.js';

const CONFIG = {
    API_BASE: 'http://localhost:8080/api/sensors',
    OFFLINE_TIMEOUT_MS: 15000 
};

let lastUpdate = null;

const elements = {
    temp: document.getElementById('temp-val'),
    hum: document.getElementById('hum-val'),
    pres: document.getElementById('pres-val'),
    statusDot: document.getElementById('status-dot'),
    statusText: document.getElementById('status-text'),
    timeText: document.getElementById('update-time'),
    buttons: document.querySelectorAll('.time-buttons button')
};

/**
 * Logic: Fetch the absolute latest reading for the top cards
 */
const updateTelemetry = async () => {
    try {
        const res = await fetch(`${CONFIG.API_BASE}/now`);
        const data = await res.json();
        
        elements.temp.innerText = `${data.temp.toFixed(1)}°C`;
        elements.hum.innerText  = `${data.humidity.toFixed(1)}%`;
        elements.pres.innerText = `${data.pressure.toFixed(0)} hPa`;
        
        lastUpdate = new Date();
        setOnlineStatus(true);
        elements.timeText.innerText = lastUpdate.toLocaleTimeString();
    } catch (err) {
        console.error("Controller Error: Telemetry fetch failed", err);
        setOnlineStatus(false);
    }
};

/**
 * Logic: Fetch the 24h list for the chart
 */
const updateHistory = async () => {
    try {
        const res = await fetch(CONFIG.API_BASE); // Standard /api/sensors list
        const history = await res.json();
        renderWeatherChart(history);
    } catch (err) {
        console.error("Controller Error: History fetch failed", err);
    }
};

const setOnlineStatus = (isOnline) => {
    elements.statusDot.style.backgroundColor = isOnline ? '#238636' : '#f85149';
    elements.statusText.innerText = isOnline ? 'System Online' : 'System Offline';
};

/**
 * Initialize Dashboard
 */
const init = () => {
    console.info("Architect: Launching Weather Dashboard Controller...");
    
    // Immediate load
    updateTelemetry();
    updateHistory();

    // Polling backup (Fallback until WebSockets are ready)
    setInterval(updateTelemetry, 5000);

    // Watchdog: If no data for 15s, mark offline
    setInterval(() => {
        if (lastUpdate && (Date.now() - lastUpdate.getTime() > CONFIG.OFFLINE_TIMEOUT_MS)) {
            setOnlineStatus(false);
        }
    }, 2000);
};

init();
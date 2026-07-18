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

const updateTelemetry = async () => {
    try {
        const res = await fetch(`${CONFIG.API_BASE}/now`);
        if (!res.ok) throw new Error();
        const data = await res.json();
        
        elements.temp.innerText = `${data.temperature.toFixed(1)}°C`;
        elements.hum.innerText  = `${data.humidity.toFixed(1)}%`;
        elements.pres.innerText = `${data.pressure.toFixed(1)} hPa`;
        
        lastUpdate = new Date();
        setOnlineStatus(true);
        elements.timeText.innerText = lastUpdate.toLocaleTimeString();
    } catch (err) {
        setOnlineStatus(false);
    }
};

// Replace your updateHistory in app.js with this:
const updateHistory = async (range = '24h') => {
    try {
        const res = await fetch(`${CONFIG.API_BASE}?range=${range}`);
        
        if (!res.ok) {
            // This will catch 404, 500, or 403 errors
            throw new Error(`Server Error: ${res.status}`);
        }
        
        const history = await res.json();
        console.log("History Data Received:", history); // Check the data format
        
        if (Array.isArray(history)) {
            renderWeatherChart(history);
        } else {
            console.error("Data is not an array:", history);
        }
    } catch (err) {
        // This will now tell you if it's a CORS issue or a syntax error
        console.error("Fetch History failed:", err); 
    }
};

const setOnlineStatus = (isOnline) => {
    elements.statusDot.style.backgroundColor = isOnline ? '#238636' : '#f85149';
    elements.statusText.innerText = isOnline ? 'System Online' : 'System Offline';
};

const setupEventListeners = () => {
    elements.buttons.forEach(btn => {
        btn.addEventListener('click', () => {
            elements.buttons.forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
            updateHistory(btn.getAttribute('data-range'));
        });
    });
};

const init = () => {
    setupEventListeners();
    updateTelemetry();
    updateHistory('24h');
    setInterval(updateTelemetry, 5000);
    setInterval(() => {
        if (lastUpdate && (Date.now() - lastUpdate.getTime() > CONFIG.OFFLINE_TIMEOUT_MS)) {
            setOnlineStatus(false);
        }
    }, 2000);
};

// At the bottom of app.js
window.addEventListener('load', () => {
    init();
});
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
        
        elements.temp.innerText = `${data.temperature.toFixed(2)}°C`;
        elements.hum.innerText  = `${data.humidity.toFixed(2)}%`;
        elements.pres.innerText = `${data.pressure.toFixed(2)} hPa`;
        
        lastUpdate = new Date();
        setOnlineStatus(true);
        elements.timeText.innerText = lastUpdate.toLocaleTimeString();
    } catch (err) {
        setOnlineStatus(false);
    }
};

const updateHistory = async (range = '24h') => {
    try {
        const res = await fetch(`${CONFIG.API_BASE}?range=${range}`);
        if (!res.ok) throw new Error();
        const history = await res.json();
        renderWeatherChart(history);
    } catch (err) {
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
    
    setInterval(updateTelemetry, 2000);

    //offline check
    setInterval(() => {
        if (lastUpdate && (Date.now() - lastUpdate.getTime() > CONFIG.OFFLINE_TIMEOUT_MS)) {
            setOnlineStatus(false);
        }
    }, 2000);
};

window.addEventListener('load', init);
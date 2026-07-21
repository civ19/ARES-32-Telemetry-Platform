let instances = {
    tempChart: null,
    humChart: null,
    presChart: null
};

export const renderWeatherChart = (historyData) => {
    if (!historyData || historyData.length === 0) return;


    const labels = historyData.map(d => {
        const date = new Date(d.timestamp);
        return isNaN(date) ? '--:--' : date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    });

    const configs = [
        { id: 'tempChart', label: 'Temperature', data: historyData.map(d => d.temperature), color: '#58a6ff' },
        { id: 'humChart', label: 'Humidity', data: historyData.map(d => d.humidity), color: '#3fb950' },
        { id: 'presChart', label: 'Pressure', data: historyData.map(d => d.pressure), color: '#ff7b72' }
    ];

    configs.forEach(conf => {
        const canvas = document.getElementById(conf.id);
        if (!canvas) return;

        const ctx = canvas.getContext('2d');
        
        //Destroy existing instance to prevent memory leaks/glitches
        if (instances[conf.id]) {
            instances[conf.id].destroy();
        }

        instances[conf.id] = new Chart(ctx, {
            type: 'line',
            data: {
                labels: labels,
                datasets: [{
                    label: conf.label,
                    data: conf.data,
                    borderColor: conf.color,
                    backgroundColor: conf.color + '15',
                    fill: true,
                    tension: 0.4,
                    pointRadius: 2,
                    borderWidth: 2
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                layout: {
                    padding: {
                        top: 10,
                        bottom: 10,
                        left: 10,
                        right: 15
                    }
                },
                plugins: {
                    legend: { display: false }
                },
                scales: {
                    x: {
                        grid: { display: false },
                        ticks: { color: '#8b949e', maxTicksLimit: 6 }
                    },
                    y: {
                        beginAtZero: false,
                        grid: { color: '#30363d' },
                        ticks: { color: '#8b949e' }
                    }
                }
            }
        });
    });
};
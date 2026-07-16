/**
 * Charts Service - Presentation Layer
 */

let weatherChart = null;

export const renderWeatherChart = (historyData) => {
    const ctx = document.getElementById('weatherChart').getContext('2d');

    // 1. Data Transformation
    // Format timestamps for the X-axis labels
    const labels = historyData.map(d => {
        return new Date(d.timestamp).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    });

    const temps = historyData.map(d => d.temp); // Ensure this matches Java field name
    const hums = historyData.map(d => d.humidity);

    // 2. Lifecycle Management (Prevent Memory Leaks)
    if (weatherChart) {
        weatherChart.destroy();
    }

    // 3. GitHub Dark Styled Configuration
    weatherChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [
                {
                    label: 'Temperature (°C)',
                    data: temps,
                    borderColor: '#58a6ff', // GitHub Blue
                    backgroundColor: 'rgba(88, 166, 255, 0.1)',
                    yAxisID: 'y',
                    tension: 0.3,
                    pointRadius: 2
                },
                {
                    label: 'Humidity (%)',
                    data: hums,
                    borderColor: '#3fb950', // GitHub Green
                    backgroundColor: 'rgba(63, 185, 80, 0.1)',
                    yAxisID: 'y1',
                    tension: 0.3,
                    pointRadius: 2
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            interaction: { mode: 'index', intersect: false },
            scales: {
                x: {
                    grid: { display: false },
                    ticks: { color: '#8b949e', maxRotation: 0 }
                },
                y: {
                    type: 'linear',
                    display: true,
                    position: 'left',
                    ticks: { color: '#58a6ff' },
                    grid: { color: 'rgba(48, 54, 61, 0.2)' }
                },
                y1: {
                    type: 'linear',
                    display: true,
                    position: 'right',
                    ticks: { color: '#3fb950' },
                    grid: { drawOnChartArea: false }
                }
            },
            plugins: {
                legend: { labels: { color: '#e6edf3', font: { weight: '600' } } }
            }
        }
    });
};
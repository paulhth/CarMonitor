let speedChartData = {
    labels: [], // This will be populated dynamically
    datasets: [{
        label: 'Speed',
        data: [], // This will be populated with historical data
        fill: false,
        borderColor: 'rgb(75, 192, 192)',
        tension: 0.1
    }]
};

let rpmChartData = {
    labels: [], // This will be populated dynamically
    datasets: [{
        label: 'RPM',
        data: [], // This will be populated with historical data
        fill: false,
        borderColor: 'rgb(192, 75, 75)',
        tension: 0.1
    }]
};

// Define the chart options
let chartOptions = {
    type: 'line',
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        },
        maintainAspectRatio: false
    }
};
// Create the Speed chart
let speedChart = new Chart(
    document.getElementById('speedChart'),
    Object.assign({ data: speedChartData }, chartOptions)
);

// Create the RPM chart
let rpmChart = new Chart(
    document.getElementById('rpmChart'),
    Object.assign({ data: rpmChartData }, chartOptions)
);

document.addEventListener('DOMContentLoaded', function() {
    // Function to fetch sensor data
    function fetchSensorData() {
        fetch('/data')
            .then(response => response.json())
            .then(data => {
                document.getElementById('speed').textContent = data.speedCurrent + ' km/h';
                document.getElementById('rpm').textContent = data.rpmCurrent + ' RPM';
                document.getElementById('oil').textContent = data.oilTempCurrent + ' C';
                document.getElementById('fuel').textContent = data.fuelConsumptionCurrent + ' L/100km';
                // ... update other fields ...

                // Check if the history size changed
                if (speedChartData.labels.length !== data.historySize) {
                    speedChartData.labels = Array.from({length: data.historySize}, (_, i) => i);
                }

                if (rpmChartData.labels.length !== data.historySize) {
                    rpmChartData.labels = Array.from({length: data.historySize}, (_, i) => i);
                }
    
                // Assuming data.speedHistory and data.rpmHistory are arrays of numbers
                if (data.speedHistory && data.speedHistory.length) {
                    speedChartData.datasets[0].data = data.speedHistory;
                }
                if (data.rpmHistory && data.rpmHistory.length) {
                    rpmChartData.datasets[0].data = data.rpmHistory;
                }

                // Update the charts
                speedChart.update();
                rpmChart.update();
            })
            .catch(error => console.error('Unable to get sensor data:', error));
    }
    // Fetch sensor data every 1 second
    setInterval(fetchSensorData, 50);
});
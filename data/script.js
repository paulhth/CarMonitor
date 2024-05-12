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




//remove line 47

let isFetching = false;
let fetchInterval;

// Function to start or stop fetching data based on button state
function toggleDataFetch() {
    isFetching = !isFetching; // Toggle the fetching state
    const button = document.getElementById('toggleButton');
    if (isFetching) {
        // Ensure no multiple intervals are set
        if (fetchInterval) clearInterval(fetchInterval);
        fetchInterval = setInterval(fetchSensorData, 1000); // Start fetching data every second
        button.textContent = "Freeze Reading"; // Update button text
    } else {
        clearInterval(fetchInterval); // Stop the interval
        button.textContent = "Unfreeze Reading"; // Update button text
    }
}


// Function to fetch sensor data
function fetchSensorData() {
    fetch('/data')
        .then(response => response.json())
        .then(data => {
            document.getElementById('speed').textContent = data.speedCurrent + ' km/h';
            document.getElementById('rpm').textContent = data.rpmCurrent + ' RPM';
            document.getElementById('voltage').textContent = data.voltageCurrent + ' V';
            document.getElementById('cool').textContent = data.coolantTempCurrent + ' C';
            document.getElementById('oil').textContent = data.oilTempCurrent + ' C';
            document.getElementById('load').textContent = data.loadCurrent + ' %';
            document.getElementById('throttle').textContent = data.throttleCurrent + ' %';
            document.getElementById('fuel').textContent = data.fuelCurrent + ' %';
            
            // Update charts
            updateCharts(data);
        })
        .catch(error => console.error('Unable to get sensor data:', error));
}

function updateCharts(data) {
    // Update chart labels
    if (speedChartData.labels.length !== data.historySize) {
        speedChartData.labels = Array.from({length: data.historySize}, (_, i) => i);
    }
    if (rpmChartData.labels.length !== data.historySize) {
        rpmChartData.labels = Array.from({length: data.historySize}, (_, i) => i);
    }

    // Update chart data
    speedChartData.datasets[0].data = data.speedHistory;
    rpmChartData.datasets[0].data = data.rpmHistory;

    // Refresh the charts
    speedChart.update();
    rpmChart.update();
}

document.addEventListener('DOMContentLoaded', function() {
    // Initially stop data fetching until the button is pressed
    document.getElementById('toggleButton').click(); // Simulate button click to set initial state
});

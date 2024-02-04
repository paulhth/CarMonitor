document.addEventListener('DOMContentLoaded', function() {
    // Function to fetch sensor data
    function fetchSensorData() {
        fetch('/data')
            .then(response => response.json())
            .then(data => {
                document.getElementById('speed').textContent = data.speed + ' km/h';
                document.getElementById('rpm').textContent = data.rpm + ' RPM';
                document.getElementById('oil').textContent = data.oil;
                document.getElementById('fuel').textContent = data.fuel + ' L/100km';
            })
            .catch(error => console.error('Unable to get sensor data:', error));
    }

    // Fetch sensor data every 2 seconds
    setInterval(fetchSensorData, 1000);
});
#include "webhandler.h"

extern int speedVariable;
extern int rpmVariable;
extern int oilLevelVariable;
extern int fuelConsumptionVariable;

extern std::vector<float> speedHistory;
extern std::vector<int> rpmHistory;

// Initialize the WebServer on port 80
WebServerHandler::WebServerHandler() : server(80) {}

String join(const std::vector<int> &v, char separator)
{
    String result;
    for (size_t i = 0; i < v.size(); ++i)
    {
        result += String(v[i]);
        if (i < v.size() - 1)
            result += separator;
    }
    return result;
}

String join(const std::vector<float> &v, char separator)
{
    String result;
    for (size_t i = 0; i < v.size(); ++i)
    {
        result += String(v[i]);
        if (i < v.size() - 1)
            result += separator;
    }
    return result;
}

void WebServerHandler::begin()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    // Define routes
    server.on("/", [this]()
              { this->handleFile("/index.html", "text/html"); });

    server.on("/style.css", HTTP_GET, [this]()
              { this->handleFile("/style.css", "text/css"); });

    server.on("/script.js", HTTP_GET, [this]()
              { this->handleFile("/script.js", "application/javascript"); });

    server.on("/data", HTTP_GET, [this]()
              { this->handleSensorData(); });

    server.begin(); // Start the server
}

void WebServerHandler::handleClient()
{
    server.handleClient();
}

void WebServerHandler::handleFile(const String &path, const String &contentType)
{
    File file = SPIFFS.open(path, "r");
    if (!file)
    {
        Serial.println("Failed to open " + path + " for reading");
        server.send(404, "text/plain", "File Not Found");
        return;
    }

    server.streamFile(file, contentType);
    file.close();
}

void WebServerHandler::handleSensorData()
{
    // Create a JSON object with current and historical sensor data
    String jsonData = "{";
    jsonData += "\"historySize\":" + String(HISTORY_SIZE) + ",";
    jsonData += "\"speedCurrent\":" + String(speedVariable) + ",";
    jsonData += "\"rpmCurrent\":" + String(rpmVariable) + ",";
    // ... other current values ...

    // The join function must properly format the array as a JSON array
    jsonData += "\"speedHistory\":[" + join(speedHistory, ',') + "],";
    jsonData += "\"rpmHistory\":[" + join(rpmHistory, ',') + "]";
    // ... other history arrays ...

    jsonData += "}";
    server.send(200, "application/json", jsonData);
}

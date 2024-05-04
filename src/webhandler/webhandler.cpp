#include "webhandler.h"

extern uint32_t speedVariable;
extern uint32_t rpmVariable;
extern uint32_t voltageVariable;
extern float throttleVariable;
extern float coolantTempVariable;
extern float loadVariable;
extern float fuelLevelVariable;
extern float oilTempVariable;

extern std::vector<float> speedHistory;
extern std::vector<int> rpmHistory;

// Initialize the WebServer on port 80
WebServerHandler::WebServerHandler() : server(80) {}

String join(const std::vector<int> &v, char separator) // join for int elements
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

String join(const std::vector<float> &v, char separator) // join for float elements
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
    // Define routes: "/" is the root URL, HTTP_GET is the HTTP method, and the lambda function is the handler
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

/**
 * @brief Handle the client requests to the server and serve the requested files.
 *
 * @param String Constant address to the path to the requested file.
 * @param String Constant address to the content type of the requested file.
 *
 * @return void
 */
void WebServerHandler::handleFile(const String &path, const String &contentType)
{
    File file = SPIFFS.open(path, "r");
    if (!file)
    {
        Serial.println("Failed to open " + path + " for reading");
        server.send(404, "text/plain", "File Not Found!");
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
    jsonData += "\"voltageCurrent\":" + String(voltageVariable) + ",";
    jsonData += "\"coolantTempCurrent\":" + String(coolantTempVariable) + ",";
    jsonData += "\"oilTempCurrent\":" + String(oilTempVariable) + ",";
    jsonData += "\"loadCurrent\":" + String(loadVariable) + ",";
    jsonData += "\"throttleCurrent\":" + String(throttleVariable) + ",";
    jsonData += "\"fuelCurrent\":" + String(fuelLevelVariable) + ",";

    // ... other current values ...

    // The join function must properly format the array as a JSON array
    jsonData += "\"speedHistory\":[" + join(speedHistory, ',') + "],";
    jsonData += "\"rpmHistory\":[" + join(rpmHistory, ',') + "]";
    // ... other history arrays ...

    jsonData += "}";
    server.send(200, "application/json", jsonData);
}

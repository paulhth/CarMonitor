#include "webhandler.h"

extern int speedVariable;
extern int rpmVariable;
extern int oilLevelVariable;
extern int fuelConsumptionVariable;

// Initialize the WebServer on port 80
WebServerHandler::WebServerHandler() : server(80) {}

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
    // Create a JSON string with your data
    String jsonData = "{\"speed\":\"" + String(speedVariable) +
                      "\", \"rpm\":\"" + String(rpmVariable) +
                      "\", \"oil\":\"" + String(oilLevelVariable) +
                      "\", \"fuel\":\"" + String(fuelConsumptionVariable) + "\"}";
    server.send(200, "application/json", jsonData);
}
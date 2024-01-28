#include "webhandler.h"

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
              { this->handleRoot(); });
    server.begin(); // Start the server
}

void WebServerHandler::handleClient()
{
    server.handleClient();
}

void WebServerHandler::handleRoot()
{
    // Read index.html file
    File file = SPIFFS.open("/index.html", "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        server.send(404, "text/plain", "File not found");
        return;
    }

    // Send the file to the browser
    server.streamFile(file, "text/html");

    // Close the file
    file.close();
}

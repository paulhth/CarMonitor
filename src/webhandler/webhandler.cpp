#include "webhandler.h"

// Initialize the WebServer on port 80
WebServerHandler::WebServerHandler() : server(80) {}

void WebServerHandler::begin()
{
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
    String html = "<!DOCTYPE html>"
                  "<html lang='en'>"
                  "<head>"
                  "<meta charset='UTF-8'>"
                  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                  "<title>Car Monitor</title>"
                  "<style>"
                  "body { font-family: Arial, sans-serif; background-color: #f0f0f0; text-align: center; }"
                  "table { margin-left: auto; margin-right: auto; }"
                  "th, td { padding: 10px; }"
                  "</style>"
                  "</head>"
                  "<body>"
                  "<h1>Car Monitor</h1>"
                  "<table border='1'>"
                  "<tr><th>Vehicle Speed</th><td id='speed'>0 km/h</td></tr>"
                  "<tr><th>Engine RPM</th><td id='rpm'>0 RPM</td></tr>"
                  "<tr><th>Fuel Consumption</th><td id='fuel'>0 L/100km</td></tr>"
                  "</table>"
                  "</body>"
                  "</html>";

    server.send(200, "text/html", html);
}

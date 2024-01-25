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
    server.send(200, "text/html", "<h1>Car Monitor</h1>");
    server.send(200, "text/html", "<p>Status - Connected.</p>"); // not working
}

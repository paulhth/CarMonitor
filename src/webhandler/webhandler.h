#ifndef WEBSERVERHANDLER_H
#define WEBSERVERHANDLER_H

#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h> // Include SPIFFS library

class WebServerHandler
{
public:
    WebServer server;

    WebServerHandler();  // Constructor
    void begin();        // Method to initialize and start the server
    void handleClient(); // Method to handle client requests

    // Define all other methods that handle specific routes
    void handleRoot();

private:
    // Define your variables and methods for internal use
};

#endif

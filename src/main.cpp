#include "webhandler/webhandler.h"
#include "config/config.h"
#include "BluetoothSerial.h"

#include "esp_wifi.h"
#include "esp_bt.h"
#include "esp_coexist.h"

#include <Arduino.h>
#include <WiFi.h>
#include <vector>


#if (BACKEND_TESTING == false)
    #include "ELMduino.h"
    BluetoothSerial SerialBT;
    ELM327 ELM327Reader;
#endif

WebServerHandler server;

bool isConnectedWifi = false;

/**
 * @brief Setup function for the Car Monitor system. Initializes the system and connects to the WiFi and ELM327.
 * @param void
 * @return void
 */
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    DEBUG_PORT.begin(115200);

    DEBUG_PORT.println("[1] 16% - Setting ESP_COEX_PREFER_WIFI.");
    if (esp_coex_preference_set(ESP_COEX_PREFER_WIFI) != ESP_OK)
    {
        DEBUG_PORT.println("[1] 16% - ERROR - ESP_COEX_PREFER_WIFI failed. Continuing...");
    }

    WiFi.mode(WIFI_STA); // Set the WiFi mode to station mode which allows the ESP32 to act as a client to a router //POATE SE VA STERGE
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        DEBUG_PORT.println("[2] 32% - Connecting to WiFi...");
        // WiFi.begin(WIFI_SSID, WIFI_PASS);
    }

    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on = WiFi connected
    DEBUG_PORT.println("[3] 48% - Connected to WiFi");
    DEBUG_PORT.print("      IP Address: ");
    DEBUG_PORT.println(WiFi.localIP());

    server.begin(); // Call the WebServerHandler::begin -> server(80).begin() //POATE SE VA MUTA SUB BT CONFIG

#if (BACKEND_TESTING == false)
    // SerialBT.setPin("1234");
    ELM_PORT.begin("ArduHUD", true);

    DEBUG_PORT.println("[4] 64% - Connecting to OBD scanner - Phase 1");
    if (!ELM_PORT.connect("Android-Vlink"))
    {
        DEBUG_PORT.println("[4] 64% - ERROR - Couldn't connect to OBD scanner - Phase 1");
        while (1)
            ;
    }

    DEBUG_PORT.println("[5] 80% - Connecting to OBD scanner - Phase 2");
    if (!ELM327Reader.begin(ELM_PORT, true, 2000))
    {
        DEBUG_PORT.println("[5] 80% - ERROR - Couldn't connect to OBD scanner - Phase 2");
        while (1)
            ;
    }

    DEBUG_PORT.println("[6] 100% - Connected to ELM327.");
#endif
}

/**
 * @brief Loop function for the Car Monitor system. Reads the sensor data and updates the history buffers.
 * @param void
 * @return void
 */
void loop()
{

    if (WiFi.status() != WL_CONNECTED && isConnectedWifi)
    {
        DEBUG_PORT.println("\nConnecting ...\n");
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        isConnectedWifi = false;
        delay(1000);
    }

#if (BACKEND_TESTING == true) /*----------------------TESTING USE CASE----------------------*/
    oilTempVariable = 95;
    if (speedVariable >= 30)
    {
        speedVariable % 2 == 0 ? speedVariable -= 3 : speedVariable += 5;
        oilTempVariable += 3;
    }

    else
    {
        speedVariable += 1;
        oilTempVariable -= 1;
    }
    if (rpmVariable >= 2000)
        rpmVariable % 2 == 0 ? rpmVariable -= 100 : rpmVariable += 200;
    else
        rpmVariable += 100;

    if (speedVariable > 200)
        speedVariable = 0;
    if (rpmVariable > 7000)
        rpmVariable = 0;

    fuelConsumptionVariable = speedVariable * 0.5;
    updateHistory(); // Update the history buffers
    server.handleClient();
    delay(1000);

#else /*----------------------REAL USE CASE----------------------*/

    switch (obd_state)
    {
    case ENG_RPM:
        speedVariable = 0;
        rpmVariable = ELM327Reader.rpm();
        oilTempVariable = 0;
        fuelConsumptionVariable = 0;

        if (ELM327Reader.nb_rx_state == ELM_SUCCESS)
        {
            DEBUG_PORT.print("rpm: ");
            DEBUG_PORT.println(rpmVariable);
        }
        else if (ELM327Reader.nb_rx_state != ELM_GETTING_MSG)
        {
            ELM327Reader.printError();
        }

        obd_state = ENG_RPM;
        break;
    }
    updateHistory(); // Update the history buffers
    server.handleClient();
    delay(100);
#endif
}

/*
Add engineCoolantTemp and manifoldPressure.
Add point system for the driver.
*/
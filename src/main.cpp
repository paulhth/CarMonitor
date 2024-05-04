#include "webhandler/webhandler.h"
#include "config/config.h"
#include "BluetoothSerial.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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

int counter = 0;

bool isConnectedWifi = false;

/**
 * @brief Setup function for the Car Monitor system. Initializes the system and connects to the WiFi and ELM327.
 * @param void
 * @return void
 */
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    DEBUG_PORT.begin(115200);//sau 38400

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

static uint32_t speedTemp, rpmTemp;
static float voltageTemp, throttleTemp, coolantTemp, loadTemp, fuelTemp, oilTemp; 
/**
 * @brief Loop function for the Car Monitor system. Reads the sensor data and updates the history buffers.
 * @param void
 * @return void
 */
void loop()
{

#if (BACKEND_TESTING == true) /*----------------------TESTING USE CASE----------------------*/
    voltageVariable = 95;
    coolantTempVariable = 85;
    if (speedVariable >= 30)
    {
        speedVariable % 2 == 0 ? speedVariable -= 3 : speedVariable += 5;
        voltageVariable -= 3;
        coolantTempVariable -= 3;
        throttleVariable -= 0.5;
        loadVariable -= 3.0;
        oilTempVariable -= 3;
        fuelLevelVariable -= 3;
    }

    else
    {
        speedVariable += 1;
        voltageVariable -= 1;
        coolantTempVariable +=1;
        throttleVariable += 0.5;
        loadVariable += 3.0;
        oilTempVariable += 1;
        fuelLevelVariable += 1;
    }
    if (rpmVariable >= 2000){
        rpmVariable % 2 == 0 ? rpmVariable -= 100 : rpmVariable += 200;
    }
    else
        rpmVariable += 100;

    throttleVariable = speedVariable * 0.5;
    loadVariable = speedVariable * 3.0;
    updateHistory(); // Update the history buffers
    server.handleClient();
    delay(1000);

#else /*----------------------REAL USE CASE----------------------*/

    switch (obd_state)
    {
    case ENG_RPM:
        rpmVariable = ELM327Reader.rpm();
        speedVariable = speedTemp;
        voltageVariable = voltageTemp;
        throttleVariable = throttleTemp;
        coolantTempVariable = coolantTemp;
        loadVariable = loadTemp;
        fuelLevelVariable = fuelTemp;
        oilTempVariable = oilTemp;
        if (ELM327Reader.nb_rx_state == ELM_SUCCESS)
        {
            DEBUG_PORT.print("rpm: ");
            DEBUG_PORT.println(rpmVariable);
            if(rpmVariable)rpmTemp = rpmVariable;
            if(rpmTemp && !rpmVariable) rpmVariable = rpmTemp;
            delay(100);
            updateHistory(); // Update the history buffers
            server.handleClient();
        }
        else if (ELM327Reader.nb_rx_state != ELM_GETTING_MSG)
        {
            ELM327Reader.printError();
        }

        obd_state = SPEED;
        break;

    case SPEED:
        speedVariable = ELM327Reader.kph();
        rpmVariable = rpmTemp;
        voltageVariable = voltageTemp;
        throttleVariable = throttleTemp;
        coolantTempVariable = coolantTemp;
        loadVariable = loadTemp;
        fuelLevelVariable = fuelTemp;
        oilTempVariable = oilTemp;
        if(speedVariable <= 220){
            if (ELM327Reader.nb_rx_state == ELM_SUCCESS)
            {
                DEBUG_PORT.print("kph: ");
                DEBUG_PORT.println(speedVariable);
                speedTemp = speedVariable;
                delay(100);
                updateHistory(); // Update the history buffers
                server.handleClient();
            }
            else if (ELM327Reader.nb_rx_state != ELM_GETTING_MSG)
            {
                ELM327Reader.printError();
            }
        }
        obd_state = VOLTAGE;
        break;

    case VOLTAGE:
        voltageVariable = ELM327Reader.batteryVoltage();
        rpmVariable = rpmTemp;
        speedVariable = speedTemp;
        throttleVariable = throttleTemp;
        coolantTempVariable = coolantTemp;
        loadVariable = loadTemp;
        fuelLevelVariable = fuelTemp;
        oilTempVariable = oilTemp;
        if(voltageVariable <= 15.0 && voltageVariable >= 0.0){
            if (ELM327Reader.nb_rx_state == ELM_SUCCESS)
            {
                DEBUG_PORT.print("voltage: ");
                DEBUG_PORT.println(voltageVariable);
                if(voltageVariable)voltageTemp = voltageVariable;
                if(voltageTemp && !voltageVariable) voltageVariable = rpmTemp;
                delay(100);
                updateHistory(); // Update the history buffers
                server.handleClient();
            }
            else if (ELM327Reader.nb_rx_state != ELM_GETTING_MSG)
            {
                ELM327Reader.printError();
            }
        }

        obd_state = THROTTLE;
        break;

    case THROTTLE:
        throttleVariable = ELM327Reader.throttle();
        voltageVariable = voltageTemp;
        rpmVariable = rpmTemp;
        speedVariable = speedTemp;
        coolantTempVariable = coolantTemp;
        loadVariable = loadTemp;
        fuelLevelVariable = fuelTemp;
        oilTempVariable = oilTemp;
        if(throttleVariable < 100){
            if (ELM327Reader.nb_rx_state == ELM_SUCCESS)
            {
                DEBUG_PORT.print("throttle: ");
                DEBUG_PORT.println(throttleVariable);
                if(throttleVariable)throttleTemp = throttleVariable;
                if(throttleTemp && !throttleVariable) throttleVariable = throttleTemp;
                delay(100);
                updateHistory(); // Update the history buffers
                server.handleClient();
            }
            else if (ELM327Reader.nb_rx_state != ELM_GETTING_MSG)
            {
                ELM327Reader.printError();
            }
        }
        obd_state = ENG_COOLANT;
        break;

    case ENG_COOLANT:
        voltageVariable = voltageTemp;
        rpmVariable = rpmTemp;
        speedVariable = speedTemp;
        throttleVariable = throttleTemp;
        coolantTempVariable = ELM327Reader.engineCoolantTemp();
        loadVariable = loadTemp;
        fuelLevelVariable = fuelTemp;
        oilTempVariable = oilTemp;
        if(coolantTempVariable > 0 && coolantTempVariable < 150){
            if (ELM327Reader.nb_rx_state == ELM_SUCCESS)
            {
                DEBUG_PORT.print("coolantTemp: ");
                DEBUG_PORT.println(coolantTempVariable);
                if(coolantTempVariable)coolantTemp = coolantTempVariable;
                if(coolantTemp && !coolantTempVariable) coolantTempVariable = coolantTemp;
                delay(100);
                updateHistory(); // Update the history buffers
                server.handleClient();
            }
            else if (ELM327Reader.nb_rx_state != ELM_GETTING_MSG)
            {
                ELM327Reader.printError();
            }
        }

        obd_state = LOAD;
        break;

    case LOAD:
        loadVariable = ELM327Reader.engineLoad();
        voltageVariable = voltageTemp;
        rpmVariable = rpmTemp;
        speedVariable = speedTemp;
        throttleVariable = throttleTemp;
        coolantTempVariable = coolantTemp;
        fuelLevelVariable = fuelTemp;
        oilTempVariable = oilTemp;  
        if(loadVariable < 100.0 && loadVariable > 0.0){
            if (ELM327Reader.nb_rx_state == ELM_SUCCESS)
            {
                DEBUG_PORT.print("load: ");
                DEBUG_PORT.println(loadVariable);
                if(loadVariable)loadTemp = loadVariable;
                if(loadTemp && !loadVariable) loadVariable = loadTemp;
                delay(100);
                updateHistory(); // Update the history buffers
                server.handleClient();
            }
            else if (ELM327Reader.nb_rx_state != ELM_GETTING_MSG)
            {
                ELM327Reader.printError();
            }
        }
        obd_state = FUEL_LEVEL;
        break;

    case FUEL_LEVEL:
        fuelLevelVariable = ELM327Reader.fuelLevel();
        voltageVariable = voltageTemp;
        rpmVariable = rpmTemp;
        speedVariable = speedTemp;
        throttleVariable = throttleTemp;
        coolantTempVariable = coolantTemp;
        loadVariable = loadTemp;
        oilTempVariable = oilTemp;
        if(fuelLevelVariable < 100.0 && fuelLevelVariable > 0.0){
            if (ELM327Reader.nb_rx_state == ELM_SUCCESS)
            {
                DEBUG_PORT.print("fuelLevel: ");
                DEBUG_PORT.println(fuelLevelVariable);
                if(fuelLevelVariable)fuelTemp = fuelLevelVariable;
                if(fuelTemp && !fuelLevelVariable) fuelLevelVariable = fuelTemp;
                delay(100);
                updateHistory(); // Update the history buffers
                server.handleClient();
            }
            else if (ELM327Reader.nb_rx_state != ELM_GETTING_MSG)
            {
                ELM327Reader.printError();
            }
        }
        obd_state = OIL_TEMP;
        break;

    case OIL_TEMP:
        oilTempVariable = ELM327Reader.oilTemp();
        voltageVariable = voltageTemp;
        rpmVariable = rpmTemp;
        speedVariable = speedTemp;
        throttleVariable = throttleTemp;
        coolantTempVariable = coolantTemp;
        loadVariable = loadTemp;
        fuelLevelVariable = fuelTemp;
        if(oilTempVariable < 150.0 && oilTempVariable > 0.0){
            if (ELM327Reader.nb_rx_state == ELM_SUCCESS)
            {
                DEBUG_PORT.print("oilTemp: ");
                DEBUG_PORT.println(oilTempVariable);
                if(oilTempVariable)oilTemp = oilTempVariable;
                if(oilTemp && !oilTempVariable) oilTempVariable = oilTemp;
                delay(100);
                updateHistory(); // Update the history buffers
                server.handleClient();
            }
            else if (ELM327Reader.nb_rx_state != ELM_GETTING_MSG)
            {
                ELM327Reader.printError();
            }
        }
        obd_state = ENG_RPM;
        break;
    }
#endif
}

/*
Add point system for the driver.

works:
kph !
rpm !
battery voltage (float) !
engine coolant temp !
oil temp !
engine load !
throttle !
fuel level 

doesnt work:
fuelrate
oiltemp
vin
manifold pressure
fuel pressure

oilTempVariable -> voltageVariable
fuelConsumptionVariable -> throttleVariable
intakePressureVariable -> loadVariable

*/
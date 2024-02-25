#include "webhandler/webhandler.h"
#include "config/config.h"
#include "bthandler/bthandler.h"

#include <Arduino.h>

#if (BACKEND_TESTING == false)
#include "ELMduino.h"
#endif

#include <WiFi.h>
#include <vector>

WebServerHandler server;

#if (BACKEND_TESTING == false)
ELM327 ELM327Reader;
#endif

bool isConnectedWifi = false;
bool isConnectedBT = false;

int speedVariable;
float rpmVariable;
float oilTempVariable;
int fuelConsumptionVariable;

std::vector<float> speedHistory(HISTORY_SIZE, 0);
std::vector<int> rpmHistory(HISTORY_SIZE, 0);
// ... other variables ...

int historyIndex = 0;

void updateHistory()
{
  // Add the latest values to the end of the history vectors
  speedHistory.push_back(speedVariable);
  rpmHistory.push_back(rpmVariable);
  // ... other variables ...

  // If we've reached the maximum history size, remove the oldest value
  if (speedHistory.size() > HISTORY_SIZE)
  {
    speedHistory.erase(speedHistory.begin());
  }
  if (rpmHistory.size() > HISTORY_SIZE)
  {
    rpmHistory.erase(rpmHistory.begin());
  }
  // ... repeat for other history vectors ...
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on = WiFi connected
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Start the server

#if (BACKEND_TESTING == false)
  SerialBT.begin("CarMonitor"); // Bluetooth device name

  if (!ELM327Reader.begin(SerialBT))
  {
    Serial.println("Failed to connect to the ELM327");
    isConnectedBT = false;
  }
  else
  {
    Serial.println("Connected to the ELM327");
    isConnectedBT = true;
  }
  delay(1000); // Allow data to start flowing from the ELM327 to the ESP32
#endif
}

void loop()
{

  if (WiFi.status() != WL_CONNECTED && isConnectedWifi)
  {
    Serial.println("\nConnecting ...\n");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    isConnectedWifi = false;
    delay(1000);
  }

  // Oil level and fuel consumption are typically not linear,
  // so you would update them based on your specific logic.

#if (BACKEND_TESTING == true) /*----------------------TESTING----------------------*/

  if (speedVariable >= 30)
    speedVariable % 2 == 0 ? speedVariable -= 3 : speedVariable += 5;
  else
    speedVariable += 1; // Increase speed by 1.2 km/h

  if (rpmVariable >= 2000)
    rpmVariable % 2 == 0 ? rpmVariable -= 100 : rpmVariable += 200;
  else
    rpmVariable += 100; // Increase RPM by 100

  if (speedVariable > 200)
    speedVariable = 0;
  if (rpmVariable > 7000)
    rpmVariable = 0;

  updateHistory(); // Update the history buffers
  server.handleClient();
  delay(1000);

#else /*----------------------PRODUCTION----------------------*/

  ELM327Reader.nb_rx_state == ELM_SUCCESS ? rpmVariable = ELM327Reader.rpm() : ELM327Reader.ELM_ERROR = true;
  ELM327Reader.nb_rx_state == ELM_SUCCESS ? speedVariable = ELM327Reader.kph() : ELM327Reader.ELM_ERROR = true;
  ELM327Reader.nb_rx_state == ELM_SUCCESS ? oilTempVariable = ELM327Reader.oilTemp() : ELM327Reader.ELM_ERROR = true;
  ELM327Reader.nb_rx_state == ELM_SUCCESS ? fuelConsumptionVariable = ELM327Reader.fuelRate() : ELM327Reader.ELM_ERROR = true;

  if (ELM327Reader.ELM_ERROR)
  {
    Serial.println("Error reading from ELM327:\n");
    ELM327Reader.printError();
    ELM327Reader.ELM_ERROR = false;
  }

  updateHistory(); // Update the history buffers
  server.handleClient();
  delay(1000);
#endif
}

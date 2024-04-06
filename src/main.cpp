#include "webhandler/webhandler.h"
#include "config/config.h"
#include "BluetoothSerial.h"

#include <Arduino.h>

#if (BACKEND_TESTING == false)
#include "ELMduino.h"
BluetoothSerial SerialBT;
#endif

#include <WiFi.h>
#include <vector>

WebServerHandler server;

#if (BACKEND_TESTING == false)
ELM327 ELM327Reader;
#define ELM_PORT SerialBT
#define DEBUG_PORT Serial
#endif

bool isConnectedWifi = false;
bool isConnectedBT = false;

int speedVariable;
#if (BACKEND_TESTING == true)
int rpmVariable;
#else
float rpmVariable;
#endif
int oilTempVariable;
float fuelConsumptionVariable;

std::vector<float> speedHistory(HISTORY_SIZE, 0);
std::vector<int> rpmHistory(HISTORY_SIZE, 0);

int historyIndex = 0;

void updateHistory()
{
  speedHistory.push_back(speedVariable);
  rpmHistory.push_back(rpmVariable);

  if (speedHistory.size() > HISTORY_SIZE)
  {
    speedHistory.erase(speedHistory.begin());
  }
  if (rpmHistory.size() > HISTORY_SIZE)
  {
    rpmHistory.erase(rpmHistory.begin());
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
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
  DEBUG_PORT.begin(115200);
  // SerialBT.setPin("1234");
  ELM_PORT.begin("ArduHUD", true);

  if (!ELM_PORT.connect("Android-Vlink"))
  {
    DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
    while (1)
      ;
  }

  if (!ELM327Reader.begin(ELM_PORT, true, 2000))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 2");
    while (1)
      ;
  }

  Serial.println("Connected to ELM327");
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

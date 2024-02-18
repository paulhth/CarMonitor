#include <Arduino.h>
#include <WiFi.h>
#include "webhandler/webhandler.h"

#include <vector>

#define WIFI_SSID "THE BRACELET"
#define WIFI_PASS "2010#PepsiHTH"

WebServerHandler server;

bool isConnectedWifi = false;
bool isConnectedBT = false;

int speedVariable = 0;
int rpmVariable = 0;
int oilLevelVariable = 100;
int fuelConsumptionVariable = 0;

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

  /*----------------------TESTING----------------------*/
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
}

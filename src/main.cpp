#include <Arduino.h>
#include <WiFi.h>
#include "webhandler/webhandler.h"

#include <vector>

#define WIFI_SSID "Romulus"
#define WIFI_PASS "mihaelaterezia1977"

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
  // Update the history buffers with the latest values
  speedHistory[historyIndex] = speedVariable;
  rpmHistory[historyIndex] = rpmVariable;
  // ... other variables ...

  // Move to the next index or loop back to the beginning
  historyIndex = (historyIndex + 1) % HISTORY_SIZE;
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

  speedVariable += 1.2; // Increase speed by 1.2 km/h
  rpmVariable += 100;   // Increase RPM by 100
  // Oil level and fuel consumption are typically not linear,
  // so you would update them based on your specific logic.

  // Ensure variables stay within a realistic range (for testing)
  if (speedVariable > 200)
    speedVariable = 0;
  if (rpmVariable > 7000)
    rpmVariable = 0;

  updateHistory(); // Update the history buffers
  server.handleClient();
  delay(1000);
}

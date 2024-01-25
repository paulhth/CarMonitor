#include <Arduino.h>
#include <WiFi.h>
// include bluetooth module
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define WIFI_SSID "Aghiz Love"
#define WIFI_PASS "bucatar56"

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  // initialize bluetooth serial
  if (!SerialBT.begin("ESP32test"))
  {
    Serial.println("An error occurred initializing Bluetooth");
  }
  else
  {
    Serial.println("Bluetooth initialized");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi\n");
}
bool isConnectedWifi = false;
bool isConnectedBT = false;

void loop()
{
  if (WiFi.status() == WL_CONNECTED && !isConnectedWifi)
  {
    Serial.println("Connected to WiFi\n");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.print("Gateway IP Address: ");
    Serial.println(WiFi.gatewayIP());
    digitalWrite(LED_BUILTIN, HIGH);
    isConnectedWifi = true;
  }

  if (WiFi.status() != WL_CONNECTED && isConnectedWifi)
  {
    Serial.println("\nConnecting ...\n");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    isConnectedWifi = false;
    delay(1000);
  }

  if (SerialBT.connected() && !isConnectedBT)
  {
    Serial.println("Connected to Bluetooth.\n");
    digitalWrite(LED_BUILTIN, HIGH); // Turn on the LED to indicate a connection
    isConnectedBT = true;
  }
  else if (!SerialBT.connected() && isConnectedBT)
  {
    Serial.println("Disconnected from Bluetooth.\n");
    isConnectedBT = false;
  }

  // If not connected to Bluetooth, blink the LED
  if (!isConnectedBT)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
  }
}

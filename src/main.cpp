#include <Arduino.h>
#include <WiFi.h>
#include "webhandler/webhandler.h"
// include bluetooth module
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define WIFI_SSID "Aghiz Love"
#define WIFI_PASS "bucatar56"

#define AP_SSID "ESP32" // SSID of the ESP32 access point
#define AP_PASS "12345" // Password of the ESP32 access point

WebServerHandler server;

char XML[2048]; // XML buffer - must be large enough to hold all data sent to the ESP32 WebServer

IPAddress Actual_IP;

IPAddress PageIP(192, 168, 1, 1);   // IP address of the ESP32 WebServer
IPAddress Gateway(192, 168, 0, 1);  // IP address of the router
IPAddress Subnet(255, 255, 255, 0); // Subnet mask
IPAddress ip;

bool isConnectedWifi = false;
bool isConnectedBT = false;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi\n");

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

  server.handleClient();
}

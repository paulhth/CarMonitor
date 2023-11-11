#include <Arduino.h>
#include <WiFi.h>

#define WIFI_SSID "Aghiz Love"
#define WIFI_PASS "bucatar56"

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(921600);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi\n");
}
bool isConnected = false;

void loop()
{
  if (WiFi.status() == WL_CONNECTED && !isConnected)
  {
    Serial.println("Connected to WiFi\n");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.print("Gateway IP Address: ");
    Serial.println(WiFi.gatewayIP());

    isConnected = true;
  }

  if (WiFi.status() != WL_CONNECTED && isConnected)
  {
    Serial.println("\nConnecting ...\n");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    isConnected = false;
    delay(1000);
  }

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(1000);
}

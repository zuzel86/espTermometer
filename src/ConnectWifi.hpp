
#include <ESP8266WiFi.h>

bool connectWiFi(const char* wifiSsid, const char* wifiPassword)
{
  int CONNECTION_TIMEOUT = 10000;
  unsigned long now = millis();

  // Configures static IP address
  // if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //   Serial.println("STA Failed to configure");
  // }

  //Begin WiFi
  WiFi.begin(wifiSsid, wifiPassword);
  while (WiFi.status() != WL_CONNECTED && millis() < now + CONNECTION_TIMEOUT) {
    delay(200);
    Serial.println("Connecting to WiFi ...");
    // TODOadd some LED signalization
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection timeout.");
    return false;
  }

  Serial.println("Wait 2s");
  delay(2000);

  // WiFi Connected
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  return true;
}

#include "Ota.hpp"

#include <ArduinoOTA.h>
extern ArduinoOTAClass ArduinoOTA;


Ota* Ota::ota = nullptr;

Ota::Ota() {}

Ota* Ota::getInstance()
{
    if (ota == nullptr) {
        ota = new Ota();
    }

    return ota;
}

void Ota::init(const String& ssid, const String& password) {
  Serial.println("Initializing OTA Module...");
  Serial.println(String("Connecting to network ") + ssid + ": ...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connection Failed!");
    return;
    // delay(5000);
    // ESP.restart();
  }
  Serial.println("Wyszedłem z pętli");

    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("myesp8266");

    // No authentication by default
    // ArduinoOTA.setPassword((const char *)"123");

    ArduinoOTA.onStart([]() {
        Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void Ota::handle() {
    ArduinoOTA.handle();
}
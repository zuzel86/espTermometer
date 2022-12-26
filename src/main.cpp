
#include <DallasTemperature.h>
#include <OneWire.h>

#include "ConnectWifi.hpp"
#include "FsMapStorage.hpp"
#include "Ota.hpp"
#include "pseudoThread.hpp"
#include "TemperatureStorage.hpp"
#include "WebPreview.hpp"

void getTemperature();

// On the air programming
Ota* ota = Ota::getInstance();

// Create a new web server
WebPreview web;

// DS18b20
const int ONE_WIRE_BUS_PIN = 4;
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);

// Storing temperatures
float currentTemp = 0.0;
TemperatureStorage ts;

void setup()
{
  // Initialize OTA programming
  ota->init();

  // Builtin LED pin mode
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup serial port
  const int SERIAL_PORT_SPEED = 115200;
  Serial.begin(SERIAL_PORT_SPEED);

  // ds18b20
  sensors.begin();

  // Get WiFi creentials
  FsMapStorage wifiCredentials("/credentials.txt");
  int storedPasswordsCnt = wifiCredentials.count();

  // Try to connect WiFi
  for (int i=0; i<storedPasswordsCnt; i++) {
    String wifiSsid = wifiCredentials.getSsid(i);
    Serial.print("Login: ");
    Serial.println(wifiSsid);
    String wifiPassword = wifiCredentials.getPassword(i);
    Serial.print("Haslo: ");
    Serial.println(wifiPassword);
    Serial.println(String("Łaczenie z siecią ") + wifiSsid + " ...");
    if (connectWiFi(wifiSsid.c_str(), wifiPassword.c_str())) {
      break;
    }
    Serial.println("Niepowodzenie.");
  }

  // Setup web server
  web.initServer();

  Serial.println("Fs: `");
  LittleFS.begin();

  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {
      Serial.print(dir.fileName());
      if(dir.fileSize()) {
          File f = dir.openFile("r");
          Serial.print(" size: ");
          Serial.println(f.size());
      }
  }

  LittleFS.end();
  Serial.println("----------------");
}

void loop()
{
  ota->handle();
  web.handleClient();

  // read temp
  static unsigned long threadId = getIdentifier();
  m_std_periodicallyExecute(threadId, std::bind(getTemperature), 3500);

  // store temp
  ts.updateTemperature(currentTemp);
}


void getTemperature() {
  sensors.requestTemperatures();
  currentTemp = sensors.getTempCByIndex(0);
  Serial.println(currentTemp);
}

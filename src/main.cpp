
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
const size_t MEASURE_INTERVAL = 3500;

void configureSerialPort() {
  const int SERIAL_PORT_SPEED = 115200;
  Serial.begin(SERIAL_PORT_SPEED);
  Serial.println("Serial port initilized. Port Speed: " + SERIAL_PORT_SPEED);
}

void configureBuiltinLed() {
  pinMode(LED_BUILTIN, OUTPUT);

  // Single 200ms blink
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
}

void connectWiFi(String& ssid, String& password){
  // Get WiFi creentials
  FsMapStorage wifiCredentials("/credentials.txt");
  int storedPasswordsCnt = wifiCredentials.count();

  // Try to connect WiFi
  for (int i=0; i<storedPasswordsCnt; i++) {
    ssid = wifiCredentials.getSsid(i);
    Serial.print("Login: ");
    Serial.println(ssid);
    password = wifiCredentials.getPassword(i);
    Serial.print("Haslo: ");
    Serial.println(password);
    Serial.println(String("Łaczenie z siecią ") + ssid + " ...");
    if (connectWiFi(ssid.c_str(), password.c_str())) {
      break;
    }
    Serial.println("Niepowodzenie.");
  }
}

void setup()
{
  configureSerialPort();
  configureBuiltinLed();

  // ds18b20
  sensors.begin();
  Serial.println("Termometr zainicjowany"); 

  // Initialize OTA programming
  ota->init("SyrionWiFi", "lubiepsipatrol");

  String ssid, password;
  connectWiFi(ssid, password);


  // Setup web server
  web.initServer();

  // Serial.println("Fs: `");
  // LittleFS.begin();

  // Dir dir = LittleFS.openDir("/");
  // while (dir.next()) {
  //     Serial.print(dir.fileName());
  //     if(dir.fileSize()) {
  //         File f = dir.openFile("r");
  //         Serial.print(" size: ");
  //         Serial.println(f.size());
  //     }
  // }
  // LittleFS.end();
}

void loop()
{
  // ota->handle();
  web.handleClient();

  // // read temp
  static unsigned long threadId = getIdentifier();
  executeIfTimeLeft(threadId, MEASURE_INTERVAL, std::bind(getTemperature));

  // // store temp
  ts.updateTemperature(currentTemp);
  delay(100);
}


void getTemperature() {
  sensors.requestTemperatures();
  currentTemp = sensors.getTempCByIndex(0);
  Serial.println(currentTemp);
}

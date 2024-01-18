
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Arduino.h>

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
uint8_t sensors_count = 0;
std::shared_ptr<std::vector<float>> currentTemps;
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
  sensors_count = sensors.getDS18Count();
  currentTemps = std::make_shared<std::vector<float>>(sensors_count);
  Serial.print("Liczba znalezionych termometrów: ");
  Serial.println(sensors_count);

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

  Serial.println("Inicjalizacja zakończona");
}

void loop()
{
  // ota->handle();                       // TODO odkomentować
  // web.handleClient();                  // TODO odkomentować

  // // read temp
  static unsigned long threadId = getIdentifier();
  executeIfTimeLeft(threadId, MEASURE_INTERVAL, std::bind(getTemperature), [](){}, &millis);

  // // store temp
  // ts.updateTemperature(currentTemp);   // TODO odkomentować
  delay(100);
}


void getTemperature() {
  sensors.requestTemperatures();
  
  for (int i=0; i<sensors_count; i++) {
    (*currentTemps)[i] = sensors.getTempCByIndex(i);
    Serial.print("Zmierzono temperatur(y): ");
    Serial.print((*currentTemps)[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

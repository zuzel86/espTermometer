
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Ota.hpp"

#include "webPreview.hpp"
#include "pseudoThread.hpp"
#include "TemperatureStorage.hpp"

#include <numeric>
#include <vector>

// Set WiFi credentials - Powstancow
// #define WIFI_SSID "SYRION7525"
// #define WIFI_PASS "syrion793650"

// Set WiFi credentials - Bukowina
#define WIFI_SSID "SYRION15175"
#define WIFI_PASS "syrion275006"

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
char convertedTemp = 0;
TemperatureStorage ts;

void webserverSetup(char* wifiSsid, char* wifiPassword);

void handleThermometer();
void zapis();
char convertTemp(int32_t rawTemp);

void setup() {
  // Inietialize OTA programming
  ota->init();

  // Builtin LED pin mode
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup serial port
  const int SERIAL_PORT_SPEED = 115200;
  Serial.begin(SERIAL_PORT_SPEED);

  // Setup web server
  webserverSetup(WIFI_SSID, WIFI_PASS);

  // ds18b20
  sensors.begin();
}


void loop() {
  ota->handle();
  web.handleClient();
  m_periodicallyExecute(handleThermometer, 1500);
}


void webserverSetup(char* wifiSsid, char* wifiPassword)
{
  //Begin WiFi
  WiFi.begin(wifiSsid, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.println("Connecting to WiFi ...");
    // TODOadd some LED signalization
  }

  Serial.println("Wait 2s");
  delay(2000);

  // WiFi Connected
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Start Web Server
  web.initServer();
}


void handleThermometer()
{
  sensors.requestTemperatures();

  float temperatureC = sensors.getTempCByIndex(0);
  currentTemp = temperatureC;

  char prompt[20];
  sprintf(prompt, "Temp: %02f ºC", currentTemp);
  periodicallyPrint(prompt, 2500);

  // // get temperature
  // const uint8_t DEVICE_INDEX = 0U;
  // DeviceAddress deviceAddress;
	// sensors.getAddress(deviceAddress, DEVICE_INDEX);
  // int32_t rawTemp = sensors.getTemp(deviceAddress);
  // // convert temperature
  // convertedTemp = convertTemp(rawTemp);

  // store temperature
  periodicallyExecute(zapis, 60000);
  // delay(500);
}


void zapis() {
  Serial.print("Zapis do bufora: ");
  Serial.println(currentTemp);

  ts.storeTemperature(currentTemp);
}


char convertTemp(int32_t rawTemp) {
  float tempC = sensors.rawToCelsius(rawTemp);
  char result = static_cast<char>(std::round(tempC));

  return result;
}

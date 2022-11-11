#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Ota.hpp"

#include "cbuffer.hpp"
#include "webPreview.hpp"
#include "pseudoThread.hpp"

#include <numeric>
#include <vector>

// Set WiFi credentials
// #define WIFI_SSID "SYRION7525"
// #define WIFI_PASS "syrion793650"
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
const size_t LVL1BUFFERSIZE = 240;
const size_t LVL2BUFFERSIZE = 300;
CBuffer<float> lvl1Bfr(LVL1BUFFERSIZE);
CBuffer<float> L2Buffer(LVL2BUFFERSIZE);

void handleThermometer();
void zapis();
char convertTemp(int32_t rawTemp);

void setup() {
  ota->init();

  pinMode(LED_BUILTIN, OUTPUT);

  // Setup serial port
  const int SERIAL_PORT_SPEED = 115200;
  Serial.begin(SERIAL_PORT_SPEED);

  //Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.println("Connecting to WiFi ...");
    // TODOadd some LED signalization
 
  }
  Serial.println("Wait 3s");
  delay(3000);

  // WiFi Connected
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Start Web Server
  web.initServer();

  // ds18b20
  sensors.begin();
}


void loop() {
  ota->handle();
  web.handleClient();
  m_periodicallyExecute(handleThermometer, 1500);
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

  lvl1Bfr.write(currentTemp);

//-------------------------------------
  // static int tmp = 0;
  // if (++tmp % 8) {
  //   float tmpBug[8];
  //   lvl1Bfr.read(tmpBug, 8);
  //   std::vector<float> v(tmpBug, tmpBug + 8);

  //   lvl1Bfr.toString();
  //   for (int i=0; i<8; i++) {
  //     Serial.println(v[i]);
  //   }

  //   float avg = std::accumulate(v.begin(), v.end(), 0) / 8.0;
  //   Serial.print("średnia: ");
  //   Serial.println(avg);
  //   L2Buffer.write(avg);
  // }
//-------------------------------------
}

char convertTemp(int32_t rawTemp) {
  float tempC = sensors.rawToCelsius(rawTemp);
  char result = static_cast<char>(std::round(tempC));

  return result;
}


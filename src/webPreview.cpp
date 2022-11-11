#
#include "webPreview.hpp"

#include <ESP8266WebServer.h>
#include <WString.h>
#include "cbuffer.hpp"


String generatePage();

// Create a new web server
ESP8266WebServer webserver(80);

static int cnt = 0;
extern float currentTemp;

const size_t LVL1BUFFERSIZE = 240;
const size_t LVL2BUFFERSIZE = 300;
extern CBuffer<float> lvl1Bfr;



String join(int* buffer, size_t size, const char* separator) {
  String result;
  for (size_t i=0; i<size-1; i++) {
    result += buffer[i];
    result += separator;
  }
  result += buffer[size-1];

  return result;
}

String join(float* buffer, size_t size, const char* separator) {
  String result;
  for (size_t i=0; i<size-1; i++) {
    result += buffer[i];
    result += separator;
  }
  result += buffer[size-1];

  return result;
}

// String printBuffer(cbuf &buffer, size_t size) {
String printBuffer(CBuffer<float> &buffer, size_t size) {
  float localBuffer[size];
  buffer.read(localBuffer, size);

  String str = join(localBuffer, size, ", ");
  Serial.println(str.c_str());

  return str;
}

void WebPreview::initServer() {
  webserver.on("/", rootPage);
  webserver.onNotFound(notfoundPage);
  webserver.begin();
}

void WebPreview::rootPage() {

  // lvl1Bfr.toString();

  String bufferTxt;
  bufferTxt += "Counter: ";
  bufferTxt += ++cnt;
  bufferTxt += "\nTemp: ";
  bufferTxt += currentTemp;

  String buf2 = generatePage();

  // webserver.send(200, "text/plain", bufferTxt.c_str());
  webserver.send(200, "text/html", buf2.c_str());
}

void WebPreview::notfoundPage() {
  webserver.send(404, "text/plain", "404: Not found.");
}

void WebPreview::handleClient() {
    webserver.handleClient();
}

String generatePage() {

  String temps("temps = [");
  temps += printBuffer(lvl1Bfr, static_cast<size_t>(LVL1BUFFERSIZE));
  temps += "];";
  // Serial.println(temps.c_str());

  int tempArgs[LVL1BUFFERSIZE];
  for (int i=0; i<LVL1BUFFERSIZE; i++) {
    tempArgs[i] = i;
  }

  String times("times = [");
  times += join(tempArgs, LVL1BUFFERSIZE, ", ");
  times += "];";
  // Serial.println(times.c_str());

  String content;
  content +=
            "<!DOCTYPE html>"
            "<html>"
            "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js\"></script>"
            "<body>"
            "<h1> Temperatura </h1>"
            "<canvas id=\"myChart3\" style=\"width:100%;max-width:1200px\"></canvas>"

            "</body>"

            "<script>";
  content += times;
  content += temps;
  content +=
            // "times = [];"
            // "temps = [];"

            // "function generateData(size, min, max, maxDiff) {"
            // "    current = (max + min) / 2;"

            // "    for (i=1; i<=size; i++) {"
            // "        dirChange = Math.random() > 0.5 ? -1 : 1;"
            // "        times.push(i * -10);"
            // "        temps.unshift(current = current + maxDiff * Math.random() * dirChange);"
            // "    }"
            // "    times.reverse();"
            // "}"
            // "generateData(50, 15, 27, 1);"


            "new Chart(\"myChart3\", {"
            "    type: \"line\","
            "    data: {"
            "        labels: times,"
            "        datasets: [{"
            "            data: temps,"
            "            borderColor: \"green\","
            "            fill: false,"
            "        }],"
            "    },"
            "    options: {},"
            "});"

            "</script>"

            "}";

  return content;
}

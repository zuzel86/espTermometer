#include "webPreview.hpp"

#include <ESP8266WebServer.h>
#include <WString.h>
#include "cbuffer.hpp"
#include "TemperatureStorage.hpp"

extern TemperatureStorage ts;
extern const size_t LVL1BUFFERSIZE;


String generatePage();

// Create a new web server
ESP8266WebServer webserver(80);

String join(int* buffer, size_t size, const char* separator) {      //TODO: remove - functionality moved to TemperatureStorage
  String result;
  if (size > 0) {
    for (size_t i=0; i<size-1; i++) {
      result += buffer[i];
      result += separator;
    }
    result += buffer[size-1];
  }

  return result;
}

String join(float* buffer, size_t size, const char* separator) {    //TODO: remove - functionality moved to TemperatureStorage
  String result;
  if (size > 0) {
    for (size_t i=0; i<size-1; i++) {
      result += buffer[i];
      result += separator;
    }
    result += buffer[size-1];
  }

  return result;
}

// String printBuffer(cbuf &buffer, size_t size) {
String printBuffer(CBuffer<float> &buffer, size_t size) {           //TODO: remove - functionality moved to TemperatureStorage
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
  String buf = generatePage();
  webserver.send(200, "text/html", buf.c_str());
}

void WebPreview::notfoundPage() {
  webserver.send(404, "text/plain", "404: Not found.");
}

void WebPreview::handleClient() {
    webserver.handleClient();
}

String generatePage() {
  String temps = ts.getL1BufferFormatted();
  // Serial.println(temps.c_str());

size_t bufsize = ts.getL1BufferCurrentSize();
  int tempArgs[bufsize];
  for (unsigned int i=0; i<bufsize; i++) {
    tempArgs[i] = i;
  }

  String times("times = [");
  times += join(tempArgs, bufsize, ", ");
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

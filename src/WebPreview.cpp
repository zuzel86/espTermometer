#include "WebPreview.hpp"

#include <ESP8266WebServer.h>
#include <WString.h>
#include "../lib/CBuffer/cbuffer.hpp"
#include "TemperatureStorage.hpp"
#include "LittleFS.h"
#include "stringUtils.hpp"

extern std::shared_ptr<TemperatureStorage> ts;
extern uint8_t sensors_count;


String generatePage();

// Create a new web server
ESP8266WebServer webserver(80);


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

  String current_temp = ts->getCurrentTemperatures(", ");

  String L1_payload = "L1_payload = [";
  for (uint8_t i=0; i<sensors_count; i++) {
      L1_payload += "[";
      L1_payload += ts->getL1BufferFormatted(", ", i);
      L1_payload += "],";
  }
  L1_payload += "];";

  String L2_payload = "L2_payload = [";
  for (uint8_t i=0; i<sensors_count; i++) {
      L2_payload += "[";
      L2_payload += ts->getL2BufferFormatted(", ", i);
      L2_payload += "],";
  }
  L2_payload += "];";

  String content;

  // Read content from filesystem
  LittleFS.begin();
  File _file = LittleFS.open("/page.html", "r");
  if (!_file) {
    return {"Nie znaleziono pliku page.html."};
  }

  while (_file.available()) {
      String line = _file.readStringUntil('\n');

      if (line == String("{{current_temp}}")) line = current_temp;
      if (line == String("{{L1_payload}}")) line = L1_payload;
      if (line == String("{{L2_payload}}")) line = L2_payload;

      content += line;
  }
  _file.close();
  LittleFS.end();

  return content;
}

#include "WebPreview.hpp"

#include <ESP8266WebServer.h>
#include <WString.h>
#include "cbuffer.hpp"
#include "TemperatureStorage.hpp"
#include "LittleFS.h"
#include "stringUtils.hpp"

extern TemperatureStorage ts;
extern const size_t LVL1BUFFERSIZE;


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

  String content;

  // Read content from filesystem
  LittleFS.begin();
  File _file = LittleFS.open("/page.html", "r");
  if (!_file) {
    return String("Nie znaleziono pliku page.html.");
  }

  while (_file.available()) {
      String line = _file.readStringUntil('\n');

      // Make substitutions
      if (line == String("{{times}}")) line = times;
      if (line == String("{{temps}}")) line = temps;

      content += line;
  }
  _file.close();
  LittleFS.end();

  return content;
}

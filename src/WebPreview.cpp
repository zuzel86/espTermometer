#include "WebPreview.hpp"

#include <ESP8266WebServer.h>
#include <WString.h>
#include "../lib/CBuffer/cbuffer.hpp"
#include "TemperatureStorage.hpp"
#include "LittleFS.h"
#include "stringUtils.hpp"

extern std::shared_ptr<TemperatureStorage> ts;


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
  // String temps = "temps = " + ts.getL1BufferFormatted() + ";";       // TODO Poprawić
  // String temps2 = "temps2 = " + ts.getL2BufferFormatted() + ";";     // TODO Poprawić
  String current_temp = ts->getCurrentTemperatures(", ");
  String temps = "";                                                    // TODO Usunąć
  String temps2 = "";                                                   // TODO Usunąć

//  auto bfr = ts->getL1SingleBuffer(0);

  size_t buf1size = ts->getL1BufferCurrentSize();
  size_t buf2size = ts->getL2BufferCurrentSize();
  int tempArgs[buf1size];
  for (unsigned int i=0; i<buf1size; i++) {
    tempArgs[i] = i;
  }

  int tempArgs2[buf2size];
  for (unsigned int i=0; i<buf2size; i++) {
    tempArgs2[i] = i;
  }

  String times("times = [");
  times += join(tempArgs, buf1size, ", ");
  times += "];";

  String times2("times2 = [");
  times2 += join(tempArgs2, buf2size, ", ");
  times2 += "];";

  String content;

  // Read content from filesystem
  LittleFS.begin();
  File _file = LittleFS.open("/page.html", "r");
  if (!_file) {
    return {"Nie znaleziono pliku page.html."};
  }

  while (_file.available()) {
      String line = _file.readStringUntil('\n');

      // Make substitutions
      if (line == String("{{times}}")) line = times;
      if (line == String("{{temps}}")) line = temps;
      if (line == String("{{times2}}")) line = times2;
      if (line == String("{{temps2}}")) line = temps2;
      if (line == String("{{current_temp}}")) line = current_temp;

      content += line;
  }
  _file.close();
  LittleFS.end();

  return content;
}

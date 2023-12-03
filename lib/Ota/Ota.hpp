#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

class Ota
{
protected:
    Ota();

    static Ota* ota;

public:
    Ota(Ota &other) = delete;
    void operator=(const Ota&) = delete;

    static Ota* getInstance();
    void init(const String& ssid, const String& password);
    void handle();
};

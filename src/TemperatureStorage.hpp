#pragma once

#include "cbuffer.hpp"
#include <WString.h>
// #include <ESP8266WiFi.h>                                                // for Serial - mayby will desapopear soon

const size_t LVL1BUFFERSIZE = 240;
const size_t LVL2BUFFERSIZE = 300;

class TemperatureStorage
{
  CBuffer<float> level1Buffer;
  CBuffer<float> level2Buffer;

  public:
  TemperatureStorage();
  void storeTemperature(float temperature);

  size_t getL1BufferCurrentSize();
  String getL1BufferFormatted();
  size_t getL2BufferCurrentSize();
  String getL2BufferFormatted();

  private:

  String join(int* buffer, size_t size, const char* separator);

  String join(float* buffer, size_t size, const char* separator);

};

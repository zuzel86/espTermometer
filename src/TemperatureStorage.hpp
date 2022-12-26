#pragma once

#include "cbuffer.hpp"
#include <WString.h>

const size_t LVL1BUFFERSIZE = 240;
const size_t LVL2BUFFERSIZE = 300;

/**
 * @brief Storage for temperatures given in float.
 * Currently stores the temperature in the two buffers.
 */
class TemperatureStorage
{
  CBuffer<float> level1Buffer;
  CBuffer<float> level2Buffer;

public:
  /**
   * @brief Construct a new Temperature Storage object.
   * Initializes the pseudo thread identifiers to use in L1 and L2 buffers.
   */
  TemperatureStorage();

  /**
   * @brief Sets the current temperature and updates the L1 and L2 buffers if the proper
   * amount of time since the last update for each buffer has left.
   *
   * @param temperature The new temperature to store.
   */
  void updateTemperature(float temperature);

  /**
   * @brief Returns the current size of L1 buffer
   *
   * @return size_t Current L1 buffer size.
   */
  size_t getL1BufferCurrentSize();

  /**
   * @brief Returns the current size of L2 buffer
   *
   * @return size_t Current L2 buffer size.
   */
  size_t getL2BufferCurrentSize();

  /**
   * @brief Returns L1 buffer as a String, formatted as comma separated double values in the square bracket.
   *
   * @return String Formatted L1 buffer
   */
  String getL1BufferFormatted();

  /**
   * @brief Returns L2 buffer as a String, formatted as comma separated double values in the square bracket.
   *
   * @return String Formatted L2 buffer
   */
  String getL2BufferFormatted();

private:
  /**
   * @brief Stores the current temprature in L1 buffer
   */
  void storeCurrentToL1Buffer(void);

  /**
   * @brief Stores the current temprature in L2 buffer
   */
  void storeCurrentToL2Buffer(void);

  /**
   * @brief Get the Buffer Formatted as String.
   * The String format is a comma, separated double valuse in square bracket.
   *
   * @param buffer Input buffer
   * @return String
   */
  String getBufferFormatted(CBuffer<float> &buffer);

  unsigned int l1pseudoThreadId;
  unsigned int l2pseudoThreadId;
};

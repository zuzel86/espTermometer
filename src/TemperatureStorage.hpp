#pragma once

#include <WString.h>

#include "../lib/CBuffer/cbuffer.hpp"
#include "MovingAverage.hpp"


const size_t BUFFER_UPDATE_MS_INTERVAL_L1 = 15000;    // Buffer 1 store temperature ratio in ms.
const size_t BUFFER_UPDATE_MS_INTERVAL_L2 = 60000;    // Buffer 2 store temperature ratio in ms.

const size_t RANGE_TIME_H_PLOT1 = 1;                  // Plot 2 range of hours
const size_t RANGE_TIME_H_PLOT2 = 5;                  // Plot 2 range of hours

const size_t BUFFER_SIZE_LEVEL1 = RANGE_TIME_H_PLOT1 * 60 * 60000 / BUFFER_UPDATE_MS_INTERVAL_L1;   // Size of buffer 1
const size_t BUFFER_SIZE_LEVEL2 = RANGE_TIME_H_PLOT2 * 60 * 60000 / BUFFER_UPDATE_MS_INTERVAL_L2;   // Size of buffer 2


/**
 * @brief Storage for temperatures given in float.
 * Currently stores the temperature in the two buffers.
 */
class TemperatureStorage
{
  CBuffer<float> level1Buffer;
  CBuffer<float> level2Buffer;

  MovingAverage avgTemperatureL1;
  MovingAverage avgTemperatureL2;

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
   * @brief Returns size of the L1 buffer
   *
   * @return size_t L1 buffer size.
   */
  size_t getL1BufferSize();

  /**
   * @brief Returns size of the L2 buffer
   *
   * @return size_t L2 buffer size.
   */
  size_t getL2BufferSize();

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

  /**
   * @brief Get current temperature.
   * @return Last added temperature from the L1 buffer.
  */
  String getCurrentTemperature();

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
   * @brief Stores moving average temperature to save in L1 buffer
   * in the proper moment of time.
   */
  void updateL1AvgTemperature(void);

  /**
   * @brief Stores moving average temperature to save in L2 buffer
   * in the proper moment of time.
   */
  void updateL2AvgTemperature(void);

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

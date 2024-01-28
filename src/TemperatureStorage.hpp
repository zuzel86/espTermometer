#pragma once

#include <memory>
#include <vector>

#include <WString.h>

#include "../lib/CBuffer/cbuffer.hpp"
#include "MovingAverage.hpp"

using temp_container = std::vector<float>;
using temp_container_ptr = std::shared_ptr<temp_container>;


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
  temp_container_ptr currentTemps;
  std::vector<MovingAverage> avgTemperatures;
  CBuffer<temp_container> level1Buffer;
  CBuffer<temp_container> level2Buffer;

  unsigned int l1pseudoThreadId;
  unsigned int l2pseudoThreadId;


public:
  /**
   * @brief Construct a new Temperature Storage object.
   * Initializes the pseudo thread identifiers to use in L1 and L2 buffers.
   */
  explicit TemperatureStorage(uint8_t sensors_count);

  /**
   * @brief Updates the buffers L1 and/or L2 if the proper since last update time left.
  */
  void updateBuffers();

  /**
   * @brief Sets the new temperature to buffer, then updates the avg buffer and L1/L2 bufdfers.
  */
  void setNewTemperatures(const temp_container& temps);

  /**
   * @brief Returns L1 buffer for specific thermometer.
   *
   * @param sensor_number Number of sensor on 1-wire line
   *
   * @return L1 buffer for specific sensor.
   */
  std::vector<float> getL1SingleBuffer(uint8_t sensor_number);

  /**
   * @brief Returns L1 buffer for specific thermometer.
   *
   * @param sensor_number Number of sensor on 1-wire line
   *
   * @return L1 buffer for specific sensor.
   */
  std::vector<float> getL2SingleBuffer(uint8_t sensor_number);

  /**
   * @brief Get current temperatures
   *
   * @param separator String that separates temperatures
   * .
   * @return Last temperature that was measured.
  */
  String getCurrentTemperatures(const String& separator);

  /**
   * @brief 
   *
   * @param separator
   * @param sensor_index
   * @return
   */
  String getL1BufferFormatted(const String& separator, uint8_t sensor_index);

  String getL2BufferFormatted(const String& separator, uint8_t sensor_index);

private:
  /**
   * @brief Stores the current temperature in L1 buffer
   */
  void storeCurrentToL1Buffer();

  /**
   * @brief Stores the current temperature in L2 buffer
   */
  void storeCurrentToL2Buffer();

  /**
   * @brief Stores moving average temperature to save in L1 buffer
   * in the proper moment of time.
   */
  void updateAvgTemperature();

  /**
   * @param buffer Source data buffer.
   * @param sensor_number number of temperature sensor in 1-wire line.
   * @return Measurement results from single sensor.
   */
  static std::vector<float> getSingleSensorResults(CBuffer<temp_container>& buffer, uint8_t sensor_number);

  // TODO [refactor] Przenieść w bardziej ogólne miejsce
  /**
   * @brief Format the collection of floats to string.
   *
   * @param numbers_collection Source collection.
   * @param separator Separator string
   *
   * @return Values from collection separated by separator.
   */
  static String vectorFloatToString(const std::vector<float>& numbers_collection, const String& separator);
};

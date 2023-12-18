
#include "TemperatureStorage.hpp"

#include <Arduino.h>

#include "pseudoThread.hpp"
#include "stringUtils.hpp"

float currentTemperature = 0;

  /**
   * @brief Construct a new Temperature Storage object.
   * Initializes the pseudo thread identifiers to use in L1 and L2 buffers.
   */
TemperatureStorage::TemperatureStorage() : level1Buffer(BUFFER_SIZE_LEVEL1), level2Buffer(BUFFER_SIZE_LEVEL2)
{
    l1pseudoThreadId = getIdentifier();
    l2pseudoThreadId = getIdentifier();
}

/**
 * @brief Returns size of the L1 buffer
 *
 * @return size_t L1 buffer size.
 */
size_t TemperatureStorage::getL1BufferSize()
{
    return level1Buffer.getBufferSize();
}

/**
 * @brief Returns size of the L2 buffer
 *
 * @return size_t L2 buffer size.
 */
size_t TemperatureStorage::getL2BufferSize()
{
    return level2Buffer.getBufferSize();
}

/**
 * @brief Sets the current temperature and updates the L1 and L2 buffers if the proper
 * amount of time since the last update for each buffer has left.
 *
 * @param temperature The new temperature to store.
 */
void TemperatureStorage::updateTemperature(float temperature)
{
    currentTemperature = temperature;

    executeIfTimeLeft(l1pseudoThreadId, BUFFER_UPDATE_MS_INTERVAL_L1,
                        std::bind(&TemperatureStorage::storeCurrentToL1Buffer, this),
                        std::bind(&TemperatureStorage::updateL1AvgTemperature, this), &millis);
    executeIfTimeLeft(l2pseudoThreadId, BUFFER_UPDATE_MS_INTERVAL_L2,
                        std::bind(&TemperatureStorage::storeCurrentToL2Buffer, this),
                        std::bind(&TemperatureStorage::updateL2AvgTemperature, this), &millis);
}

/**
 * @brief Returns the current size of L1 buffer
 *
 * @return size_t Current L1 buffer size.
 */
size_t TemperatureStorage::getL1BufferCurrentSize()
{
    return level1Buffer.getCurrentSize();
}

/**
 * @brief Returns the current size of L2 buffer
 *
 * @return size_t Current L2 buffer size.
 */
size_t TemperatureStorage::getL2BufferCurrentSize()
{
    return level2Buffer.getCurrentSize();
}

/**
 * @brief Returns L1 buffer as a String, formatted as comma separated double values in the square bracket.
 *
 * @return String Formatted L1 buffer
 */
String TemperatureStorage::getL1BufferFormatted()
{
    return getBufferFormatted(level1Buffer);
}

/**
 * @brief Returns L2 buffer as a String, formatted as comma separated double values in the square bracket.
 *
 * @return String Formatted L2 buffer
 */
String TemperatureStorage::getL2BufferFormatted()
{
    return getBufferFormatted(level2Buffer);
}

String TemperatureStorage::getCurrentTemperature()
{
    auto temp = level1Buffer.read();

    return String(temp);
}


//************************* Private members *************************

/**
 * @brief Stores the average temprature in L1 buffer,
 * and resets average temperature buffer.
 */
void TemperatureStorage::storeCurrentToL1Buffer(void)
{
    updateL1AvgTemperature();
    level1Buffer.write(avgTemperatureL1.getCurrentValue());
    avgTemperatureL1.reset();
}

/**
 * @brief Stores the average temprature in L2 buffer,
 * and resets average temperature buffer.
 */
void TemperatureStorage::storeCurrentToL2Buffer(void)
{
    updateL2AvgTemperature();
    level2Buffer.write(avgTemperatureL2.getCurrentValue());
    avgTemperatureL2.reset();
}

/**
 * @brief Stores moving average temperature to save in L1 buffer
 * in the proper moment of time.
 */
void TemperatureStorage::updateL1AvgTemperature(void)
{
    avgTemperatureL1.update(currentTemperature);
}

/**
 * @brief Stores moving average temperature to save in L2 buffer
 * in the proper moment of time.
 */
void TemperatureStorage::updateL2AvgTemperature(void)
{
    avgTemperatureL2.update(currentTemperature);
}

/**
 * @brief Get the Buffer Formatted as String.
 * The String format is a comma, separated double valuse in square bracket.
 *
 * @param buffer Input buffer
 * @return String
 */
String TemperatureStorage::getBufferFormatted(CBuffer<float> &buffer)
{
    size_t size = buffer.getCurrentSize();
    float localBuffer[size];
    buffer.read(localBuffer, size);
    String str = join(localBuffer, size, ", ");

    String temps ("[");
    temps += str;
    temps += "];";

    return temps;
}

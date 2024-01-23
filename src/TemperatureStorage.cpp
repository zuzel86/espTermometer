
#include "TemperatureStorage.hpp"

#include <Arduino.h>

#include "pseudoThread.hpp"
#include "stringUtils.hpp"

temp_container_ptr currentTemperatures;         // TODO Zastanowić się czy to nie powinno być schowane w obiekcie

  /**
   * @brief Construct a new Temperature Storage object.
   * Initializes the pseudo thread identifiers to use in L1 and L2 buffers.
   */
TemperatureStorage::TemperatureStorage(uint8_t sensors_count) : level1Buffer(BUFFER_SIZE_LEVEL1), 
                                                                level2Buffer(BUFFER_SIZE_LEVEL2)
{
    l1pseudoThreadId = getIdentifier();
    l2pseudoThreadId = getIdentifier();
    for (uint8_t i=0; i<sensors_count; i++) {
        avgTemperatures.emplace_back(3);        // TODO Sprawdzić czemu nie działa z zerem
    }
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
 * @brief Sets the current temperatures and updates the L1 and L2 buffers if the proper
 * amount of time since the last update for each buffer has left.
 *
 * @param temperatures The new temperatures to store.
 */
void TemperatureStorage::updateTemperature(const temp_container_ptr& temperatures)
{
    currentTemperatures = temperatures;
    updateAvgTemperature();

    executeIfTimeLeft(l1pseudoThreadId, BUFFER_UPDATE_MS_INTERVAL_L1,
                      [this] { storeCurrentToL1Buffer(); }, [] {}, &millis);
    executeIfTimeLeft(l2pseudoThreadId, BUFFER_UPDATE_MS_INTERVAL_L2,
                      [this] { storeCurrentToL2Buffer(); }, [] {}, &millis);
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
// String TemperatureStorage::getL1BufferFormatted()
// {
//     return getBufferFormatted(level1Buffer);     // TODO Do usunięcia
// }

/**
 * @brief Returns L2 buffer as a String, formatted as comma separated double values in the square bracket.
 *
 * @return String Formatted L2 buffer
 */
// String TemperatureStorage::getL2BufferFormatted()    // TODO Do usunięcia
// {
//     return getBufferFormatted(level2Buffer);
// }

 String TemperatureStorage::getCurrentTemperatures(const String& separator)
 {
     return vectorFloatToString(*currentTemperatures, separator);
 }


//************************* Private members *************************

/**
 * @brief Stores the average temperature in L1 buffer,
 * and resets average temperature buffer.
 */
void TemperatureStorage::storeCurrentToL1Buffer()
{
    temp_container temps;
    for (auto& avgTempBuffer : avgTemperatures) {
        temps.push_back(avgTempBuffer.getCurrentValue());
        avgTempBuffer.reset();
    }
    level1Buffer.write(temps);
}

/**
 * @brief Stores the average temperature in L2 buffer,
 * and resets average temperature buffer.
 */
void TemperatureStorage::storeCurrentToL2Buffer()
{
    temp_container temps;
    for (auto& avgTempBuffer : avgTemperatures) {
        temps.push_back(avgTempBuffer.getCurrentValue());
        avgTempBuffer.reset();
    }
    level2Buffer.write(temps);
}

/**
 * @brief Stores moving average temperature to save in L1 buffer
 * in the proper moment of time.
 */
void TemperatureStorage::updateAvgTemperature()
{
    auto temperature_it = currentTemperatures->begin();
    for (auto& avgTemp : avgTemperatures) {
        avgTemp.update(*temperature_it);
        temperature_it++;
    }
    assert(temperature_it == currentTemperatures->end());       // Wszystkie pomiary zostały przepisane
}

std::vector<float> TemperatureStorage::getL1SingleBuffer(uint8_t sensor_number) {
    return getSingleSensorResults(level1Buffer, sensor_number);
}

std::vector<float> TemperatureStorage::getL2SingleBuffer(uint8_t sensor_number) {
    return getSingleSensorResults(level2Buffer, sensor_number);
}

// TODO Dodać const-iterator do CBuffer i zmienić na const
std::vector<float> TemperatureStorage::getSingleSensorResults(CBuffer<temp_container> &buffer, uint8_t sensor_number) {
    auto result = std::vector<float>();
    for (auto& results : buffer) {            
        result.push_back(results[sensor_number]);
    }

    std::reverse(result.begin(), result.end());
    return result;
}

String TemperatureStorage::vectorFloatToString(const std::vector<float>& numbers_collection, const String& separator) {
    auto result = String();
    for(const auto& sensor_n : numbers_collection) {
//        if (&sensor_n != &*(*currentTemperatures).begin()) {
//            result += separator;
//        } TODO zobaczyć czemu nie działa
        result += sensor_n;
        result += separator;
    }

    return result;
}

String TemperatureStorage::getL1BufferFormatted(const String &separator, uint8_t sensor_index) {
    return vectorFloatToString(getSingleSensorResults(level1Buffer, sensor_index), separator);
}

String TemperatureStorage::getL2BufferFormatted(const String &separator, uint8_t sensor_index) {
    return vectorFloatToString(getSingleSensorResults(level2Buffer, sensor_index), separator);
}

/**
 * @brief Stores moving average temperature to save in L2 buffer
 * in the proper moment of time.
 */
// void TemperatureStorage::updateL2AvgTemperature()
// {
//     Serial.println("updateL2AvgTemperature()");

//     avgTemperatureL2.update(currentTemperatures);
// }

/**
 * @brief Get the Buffer Formatted as String.
 * The String format is a comma, separated double valuse in square bracket.
 *
 * @param buffer Input buffer
 * @return String
 */
//String TemperatureStorage::getBufferFormatted(CBuffer<float> &buffer)
//{
//    size_t size = buffer.getCurrentSize();
//    float localBuffer[size];
//    buffer.read(localBuffer, size);
//    String str = join(localBuffer, size, ", ");
//
//    String temps ("[");
//    temps += str;
//    temps += "];";
//
//    return temps;
//}

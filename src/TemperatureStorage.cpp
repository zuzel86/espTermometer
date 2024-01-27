
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



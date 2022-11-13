
#include "TemperatureStorage.hpp"

TemperatureStorage::TemperatureStorage() : level1Buffer(LVL1BUFFERSIZE), level2Buffer(LVL2BUFFERSIZE)
{
}

void TemperatureStorage::storeTemperature(float temperature)
{
    level1Buffer.write(temperature);
}

size_t TemperatureStorage::getL1BufferCurrentSize()
{
    return level1Buffer.getCurrentSize();
}

String TemperatureStorage::getL1BufferFormatted()
{
    size_t size = level1Buffer.getCurrentSize();
    float localBuffer[size];
    level1Buffer.read(localBuffer, size);
    String str = join(localBuffer, size, ", ");

    String temps("temps = [");
    temps += str;
    temps += "];";

    return temps;
}

size_t TemperatureStorage::getL2BufferCurrentSize()
{
    return level2Buffer.getCurrentSize();
}

String TemperatureStorage::getL2BufferFormatted()
{
    String temps("temps = [");
    temps += "];";

    return temps;
}

//************************* Privatr members *************************

String TemperatureStorage::join(int* buffer, size_t size, const char* separator)
{
    String result;
    if (size > 0) {
        for (size_t i=0; i<size-1; i++) {
            result += buffer[i];
            result += separator;
        }
        result += buffer[size-1];
    }

    return result;
}

String TemperatureStorage::join(float* buffer, size_t size, const char* separator)
{
    String result;
    if (size > 0) {
        for (size_t i=0; i<size-1; i++) {
            result += buffer[i];
            result += separator;
        }
        result += buffer[size-1];
    }

    return result;
}

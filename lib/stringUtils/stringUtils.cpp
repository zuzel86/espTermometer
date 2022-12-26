
#include "stringUtils.hpp"

String join(int* buffer, size_t size, const char* separator)
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

String join(float* buffer, size_t size, const char* separator)
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

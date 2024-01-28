#pragma once

#include <FS.h>
#include "LittleFS.h"

#include <map>
#include <string>
#include <vector>

/**
 * @brief Stores a map of <String, String> values in LittleFS filesystem.
 */
class FsMapStorage
{

public:
    explicit FsMapStorage(const char* srcPath);

    /**
     * @brief Get the number of stored WiFi credentials.
     * 
     * @return number of credentials
    */
    [[nodiscard]] int count() const;

    /**
     * @brief Gets the WiFi SSID at the given index.
     * 
     * @param index Index of WiFi SSID to return.
     * @return const String& SSID
    */
    [[nodiscard]] const String& getSsid(int index) const;

    /**
     * @brief Gets the WiFi Password at the given index.
     *
     * @param index Index of WiFi pssword to return.
     * @return const String& Password
     */
    const String& getPassword(int index) const;

private:
    std::pair<String, String> explode(String line);
    void parse(const char* fileName);

    std::map<String, String>  storage_;
};

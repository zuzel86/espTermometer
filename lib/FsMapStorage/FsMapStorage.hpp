#pragma once

#include <FS.h>
#include <vector>
#include <map>
#include <string>

#include "LittleFS.h"

/**
 * @brief Stores a map of <String, String> values in LittleFS filesystem.
 */
class FsMapStorage
{

public:
    FsMapStorage(const char* srcPath);
    int count() const;
    const String& getSsid(int index) const;

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

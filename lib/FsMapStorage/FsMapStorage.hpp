#pragma once

#include <FS.h>
#include <vector>
#include <map>
#include <string>

#include "LittleFS.h"

class FsMapStorage
{

public:
    FsMapStorage(const char* srcPath);
    int count();
    String getSsid(int index);
    String getPassword(int index);

private:
    std::pair<String, String> explode(String line);
    void parse(const char* fileName);

    std::map<String, String>  storage;
};

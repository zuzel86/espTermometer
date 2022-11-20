
#include "FsMapStorage.hpp"


FsMapStorage::FsMapStorage(const char* srcPath) {
    parse(srcPath);
}

int FsMapStorage::count() {
    return storage.size();
}

String FsMapStorage::getSsid(int index) {
    auto it = storage.begin();
    std::advance(it, index);

    return it->first;
}

String FsMapStorage::getPassword(int index) {
    auto it = storage.begin();
    std::advance(it, index);

    return it->second;
}

// PRIVATE MEMBERS

std::pair<String, String> FsMapStorage::explode(String line) {

    // Trivial implementation, assumes a sunny day scenario only (no colons in ssid and passwd, correct structure)
    // TODO Add more safe implementation.
    std::pair<String, String> sp;
    int colonPos = line.indexOf(':');

    sp.first = line.substring(0, colonPos);
    sp.second = line.substring(colonPos + 1);

    return sp;
}

void FsMapStorage::parse(const char* fileName) {
    File _file = LittleFS.open(fileName, "r");
    while (_file.available()) {
        String s = _file.readStringUntil('\n');
        std::pair sp = explode(s);

        storage[sp.first] = sp.second;
    }
    _file.close();
}

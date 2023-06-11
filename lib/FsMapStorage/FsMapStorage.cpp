
#include "FsMapStorage.hpp"

/**
 * @brief Construct a new FsMapStorage object and fill the storage_meeber, with dhe data from the given file.
 *
 * @param srcPath LittleFS file path.
 */
FsMapStorage::FsMapStorage(const char* srcPath) {
    parse(srcPath);
}

/**
 * @brief Returns the amount of current stored <SSID, Passwd> pairs.
 *
 * @return int Amount of stored pairs.
 */
int FsMapStorage::count() const {
    return storage_.size();
}

/**
 * @brief Gets the SSID at the given index.
 *
 * @param index Index of SSID to return.
 * @return const String& SSID
 */
const String& FsMapStorage::getSsid(int index) const {
    auto it = storage_.begin();
    std::advance(it, index);

    return it->first;
}

const String& FsMapStorage::getPassword(int index) const {
    auto it = storage_.begin();
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
/**
 * @brief Read available passowrds from file and stores it in the storage_ member.
 *
 * @param fileName Name of file to read.
 */
void FsMapStorage::parse(const char* fileName) {
    bool fsMountedSuccess = LittleFS.begin();       // TODO: Interpret the returned value in the future
    File _file = LittleFS.open(fileName, "r");
    while (_file.available()) {
        String s = _file.readStringUntil('\n');
        std::pair sp = explode(s);

        storage_[sp.first] = sp.second;             // TODO: We not need to use _storage here, we can return a map or use a reference passed by arg.
    }
    _file.close();
    LittleFS.end();
}

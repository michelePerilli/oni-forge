#include "XmlReader.hpp"
#include "XmlDocument.hpp"

#include <iostream>

bool XmlReader::read(const std::string& filePath, XmlDocument& document) {
    if (!document.loadFromFile(filePath)) {
        std::cerr << "[XmlReader] Failed to read file: " << filePath
                  << " — " << document.getLastError() << "\n";
        return false;
    }

    return true;
}
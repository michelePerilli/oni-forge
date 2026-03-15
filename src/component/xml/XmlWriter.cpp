#include "XmlWriter.hpp"
#include "XmlDocument.hpp"

#include <iostream>

bool XmlWriter::write(const XmlDocument& document, const std::string& filePath) {
    if (!document.isLoaded()) {
        std::cerr << "[XmlWriter] Cannot write an unloaded document to: " << filePath << "\n";
        return false;
    }

    if (!document.saveToFile(filePath)) {
        std::cerr << "[XmlWriter] Failed to write file: " << filePath << "\n";
        return false;
    }

    return true;
}
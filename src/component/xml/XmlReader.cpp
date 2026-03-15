#include "XmlReader.hpp"
#include "XmlDocument.hpp"
#include "../logger/ILogger.hpp"

XmlReader::XmlReader(ILogger& logger)
    : m_logger(logger) {}

bool XmlReader::read(const std::string& filePath, XmlDocument& document) const {
    if (!document.loadFromFile(filePath)) {
        m_logger.error("[XmlReader] Failed to read file: " + filePath + " — " + document.getLastError());
        return false;
    }

    m_logger.info("[XmlReader] File read successfully: " + filePath);
    return true;
}

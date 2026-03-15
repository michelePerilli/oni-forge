#include "component/xml/XmlWriter.hpp"
#include "component/xml/XmlDocument.hpp"
#include "component/logger/ILogger.hpp"

XmlWriter::XmlWriter(ILogger& logger)
    : m_logger(logger) {}

bool XmlWriter::write(const XmlDocument& document, const std::string& filePath) const {
    if (!document.isLoaded()) {
        m_logger.error("[XmlWriter] Cannot write an unloaded document to: " + filePath);
        return false;
    }

    if (!document.saveToFile(filePath)) {
        m_logger.error("[XmlWriter] Failed to write file: " + filePath);
        return false;
    }

    m_logger.info("[XmlWriter] File written successfully: " + filePath);
    return true;
}

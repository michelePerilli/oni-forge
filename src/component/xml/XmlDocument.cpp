#include "XmlDocument.hpp"

bool XmlDocument::loadFromFile(const std::string& filePath) {
    const pugi::xml_parse_result result = m_document.load_file(
        filePath.c_str(),
        // Preserve whitespace and original formatting as much as possible
        pugi::parse_default | pugi::parse_ws_pcdata
    );

    if (!result) {
        m_loaded = false;
        m_lastError = result.description();
        return false;
    }

    m_loaded = true;
    m_lastError.clear();
    return true;
}

bool XmlDocument::saveToFile(const std::string& filePath) const {
    // pugi::format_raw avoids adding extra indentation or newlines,
    // preserving the original document structure as closely as possible
    const bool success = m_document.save_file(
        filePath.c_str(),
        "  ",                // indent string
        pugi::format_default // change to format_raw if whitespace must be untouched
    );

    return success;
}

bool XmlDocument::isLoaded() const {
    return m_loaded;
}

const std::string& XmlDocument::getLastError() const {
    return m_lastError;
}

pugi::xml_document& XmlDocument::getRawDocument() {
    return m_document;
}

const pugi::xml_document& XmlDocument::getRawDocument() const {
    return m_document;
}
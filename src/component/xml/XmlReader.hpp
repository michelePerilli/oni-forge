#pragma once

#include <string>

// Forward declaration to avoid including XmlDocument.hpp here
class XmlDocument;

/**
 * @brief Responsible for reading XML files from disk into an XmlDocument.
 *
 * XmlReader is a stateless service. It holds no data between calls.
 * All resulting state lives in the XmlDocument passed by the caller.
 */
class XmlReader {
public:
    XmlReader() = default;

    /**
     * @brief Reads an XML file from disk and populates the given document.
     * @param filePath Absolute or relative path to the XML file.
     * @param document An empty XmlDocument to be populated.
     * @return True if the file was read successfully, false otherwise.
     */
    bool read(const std::string& filePath, XmlDocument& document);
};
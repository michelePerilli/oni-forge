#pragma once

#include <string>

// Forward declarations
class XmlDocument;
class Logger;

/**
 * @brief Responsible for reading XML files from disk into an XmlDocument.
 *
 * XmlReader is a stateless service. It holds no data between calls.
 * All resulting state lives in the XmlDocument passed by the caller.
 */
class XmlReader {
public:
    /**
     * @brief Constructs an XmlReader with the given logger.
     * @param logger The logger to use for reporting errors and progress.
     */
    explicit XmlReader(Logger& logger);

    /**
     * @brief Reads an XML file from disk and populates the given document.
     * @param filePath Absolute or relative path to the XML file.
     * @param document An empty XmlDocument to be populated.
     * @return True if the file was read successfully, false otherwise.
     */
    [[nodiscard]] bool read(const std::string& filePath, XmlDocument& document) const;

private:
    Logger& m_logger;
};

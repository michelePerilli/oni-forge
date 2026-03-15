#pragma once

#include <string>

// Forward declaration to avoid including XmlDocument.hpp here
class XmlDocument;

/**
 * @brief Responsible for writing an XmlDocument to disk.
 *
 * XmlWriter is a stateless service. It holds no data between calls.
 * Overwriting an existing file is intentional and expected behavior.
 */
class XmlWriter {
public:
    XmlWriter() = default;

    /**
     * @brief Writes the given document to disk, overwriting any existing file.
     * @param document The document to write.
     * @param filePath Absolute or relative path to the destination file.
     * @return True if the file was written successfully, false otherwise.
     */
    bool write(const XmlDocument& document, const std::string& filePath);
};
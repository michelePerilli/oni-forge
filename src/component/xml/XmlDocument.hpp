#pragma once

#include <pugixml.hpp>
#include <string>

/**
 * @brief Wrapper around pugi::xml_document.
 *
 * This class is the only point of contact with the pugixml library.
 * All other classes must depend on XmlDocument instead of using
 * pugixml types directly.
 */
class XmlDocument {
public:
    XmlDocument() = default;

    // Not copyable: copying an xml document is expensive and rarely intentional
    XmlDocument(const XmlDocument&) = delete;

    XmlDocument& operator=(const XmlDocument&) = delete;

    // Movable
    XmlDocument(XmlDocument&&) = default;

    XmlDocument& operator=(XmlDocument&&) = default;

    /**
     * @brief Loads an XML document from a file.
     * @param filePath Absolute or relative path to the XML file.
     * @return True if the file was loaded successfully, false otherwise.
     */
    [[nodiscard]] bool loadFromFile(const std::string& filePath);

    /**
     * @brief Marks the document as loaded.
     *
     * Used when the document is built programmatically instead of
     * being loaded from a file, so that XmlWriter can write it to disk.
     */
    void markAsLoaded();

    /**
     * @brief Saves the current document to a file.
     *        Tag order is preserved as-is from the in-memory document.
     * @param filePath Absolute or relative path to the destination file.
     * @return True if the file was saved successfully, false otherwise.
     */
    [[nodiscard]] bool saveToFile(const std::string& filePath) const;

    /**
     * @brief Returns whether the document has been loaded successfully.
     */
    [[nodiscard]] bool isLoaded() const;

    /**
     * @brief Returns the last error message, if any.
     */
    [[nodiscard]] const std::string& getLastError() const;

    /**
     * @brief Provides raw access to the underlying pugi::xml_document.
     *
     * This method should only be used by XmlReader and XmlWriter.
     * Do NOT use this in ModManager or other high-level classes.
     */
    [[nodiscard]] pugi::xml_document& getRawDocument();

    [[nodiscard]] const pugi::xml_document& getRawDocument() const;

private:
    pugi::xml_document m_document;
    bool m_loaded = false;
    std::string m_lastError;
};

#pragma once

#include <optional>
#include <string>

#include "component/xml/XmlDocument.hpp"
#include "model/ONCC.hpp"
#include "model/OniFile.hpp"

// Forward declarations
class XmlReader;
class XmlWriter;
class ILogger;

/**
 * @brief Repository for managing Character Class (ONCC) data persistence.
 *
 * This class handles the reading and writing of ONCC files to and from the disk.
 * It abstracts the XML parsing and serialization logic, providing a high-level
 * interface for loading and saving ONCC data structures.
 *
 * It utilizes:
 * - XmlReader: To read raw XML files.
 * - XmlWriter: To write data back to XML format.
 * - ILogger: For logging errors and operations.
 */
class ONCCRepository {
public:
    /**
     * @brief Constructs a new ONCCRepository.
     *
     * @param reader Reference to the XML reading service.
     * @param writer Reference to the XML writing service.
     * @param logger Reference to the logging service.
     */
    ONCCRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger);

    /**
     * @brief Loads an ONCC file from the specified path.
     *
     * attempts to read the file at the given path, parse its XML content,
     * and map it to an ONCC::Root object.
     *
     * @param filePath The absolute or relative path to the ONCC XML file.
     * @return std::optional<OniFile<ONCC::Root>> Containing the loaded file data if successful,
     *         or std::nullopt if the file could not be read or parsed.
     */
    [[nodiscard]] std::optional<OniFile<ONCC::Root>> load(const std::string& filePath) const;

    /**
     * @brief Saves an ONCC file to disk.
     *
     * Serializes the provided ONCC data structure into XML format and writes it
     * to the file path specified within the OniFile object.
     *
     * @param file The OniFile object containing the ONCC data and the target file path.
     * @return true If the file was successfully written.
     * @return false If serialization failed or the file could not be written.
     */
    [[nodiscard]] bool save(const OniFile<ONCC::Root>& file) const;

private:
    const XmlReader& m_reader; ///< Service for reading XML files.
    const XmlWriter& m_writer; ///< Service for writing XML files.
    const ILogger&   m_logger; ///< Logger for error reporting.

    /**
     * @brief Parses an XML document into an ONCC::Root structure.
     *
     * Internal helper that maps the raw XML DOM to the strongly-typed ONCC model.
     *
     * @param document The loaded XML document.
     * @return std::optional<ONCC::Root> The parsed root object, or std::nullopt on mapping failure.
     */
    [[nodiscard]] std::optional<ONCC::Root> parseDocument(const XmlDocument& document) const;

    /**
     * @brief Builds an XML document from an ONCC::Root structure.
     *
     * Internal helper that serializes the strongly-typed ONCC model back into
     * an XML DOM structure ready for writing.
     *
     * @param root The ONCC data to serialize.
     * @return XmlDocument The resulting XML document.
     */
    [[nodiscard]] static XmlDocument buildDocument(const ONCC::Root& root);
};

#pragma once

#include <optional>
#include <string>

#include "component/xml/XmlDocument.hpp"
#include "model/ONCV.hpp"
#include "model/OniFile.hpp"

// Forward declarations
class XmlReader;
class XmlWriter;
class ILogger;

/**
 * @brief Responsible for reading and writing ONCV files.
 *
 * Depends on XmlReader, XmlWriter and ILogger injected via constructor.
 * Knows how to map between the ONCV struct and its XML representation.
 */
class ONCVRepository {
public:
    ONCVRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger);

    /**
     * @brief Reads an ONCV file from disk and returns the parsed struct.
     * @param filePath Absolute or relative path to the ONCV XML file.
     * @return The parsed ONCV struct, or std::nullopt if reading or parsing failed.
     */
    [[nodiscard]] std::optional<OniFile<ONCV::Root>> load(const std::string& filePath) const;

    /**
     * @brief Writes an ONCV file to disk using the path stored in the OniFile wrapper.
     * @param file The ONCV data and its associated file path.
     * @return True if the file was written successfully, false otherwise.
     */
    [[nodiscard]] bool save(const OniFile<ONCV::Root>& file) const;

private:
    const XmlReader& m_reader;
    const XmlWriter& m_writer;
    const ILogger& m_logger;

    /**
     * @brief Parses an ONCV struct from a loaded XmlDocument.
     * @param document A successfully loaded XmlDocument.
     * @return The parsed ONCV struct, or std::nullopt if the structure is unexpected.
     */
    [[nodiscard]] std::optional<ONCV::Root> parseDocument(const XmlDocument& document) const;

    /**
     * @brief Serializes an ONCV struct into a new XmlDocument.
     * @param oncv The ONCV struct to serialize.
     * @return A populated XmlDocument ready to be written to disk.
     */
    [[nodiscard]] static XmlDocument buildDocument(const ONCV::Root& oncv);
};

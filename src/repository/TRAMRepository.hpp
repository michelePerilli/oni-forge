#pragma once

#include <optional>
#include <string>

#include "component/xml/XmlDocument.hpp"
#include "model/OniFile.hpp"
#include "model/TRAM.hpp"

// Forward declarations
class XmlReader;
class XmlWriter;
class Logger;

/**
 * @brief Responsible for reading and writing TRAM files.
 *
 * Depends on XmlReader, XmlWriter and Logger injected via constructor.
 * All XML mapping logic lives in TRAMMapping.hpp.
 */
class TRAMRepository {
public:
    TRAMRepository(const XmlReader& reader, const XmlWriter& writer, const Logger& logger);

    /**
     * @brief Reads a TRAM file from disk and returns the parsed struct.
     * @param filePath Absolute or relative path to the TRAM XML file.
     * @return The parsed TRAM struct, or std::nullopt if reading or parsing failed.
     */
    [[nodiscard]] std::optional<OniFile<TRAM::Root>> load(const std::string& filePath) const;

    /**
     * @brief Writes a TRAM file to disk using the path stored in the OniFile wrapper.
     * @param file The TRAM data and its associated file path.
     * @return True if the file was written successfully, false otherwise.
     */
    [[nodiscard]] bool save(const OniFile<TRAM::Root>& file) const;

private:
    const XmlReader& m_reader;
    const XmlWriter& m_writer;
    const Logger&   m_logger;

    /**
     * @brief Parses a TRAM struct from a loaded XmlDocument.
     * @param document A successfully loaded XmlDocument.
     * @return The parsed TRAM struct, or std::nullopt if the structure is unexpected.
     */
    [[nodiscard]] std::optional<TRAM::Root> parseDocument(const XmlDocument& document) const;

    /**
     * @brief Serializes a TRAM struct into a new XmlDocument.
     * @param root The TRAM struct to serialize.
     * @return A populated XmlDocument ready to be written to disk.
     */
    [[nodiscard]] XmlDocument buildDocument(const TRAM::Root& root) const;
};

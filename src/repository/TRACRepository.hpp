#pragma once

#include <optional>
#include <string>

#include "component/xml/XmlDocument.hpp"
#include "model/OniFile.hpp"
#include "model/TRAC.hpp"

// Forward declarations
class XmlReader;
class XmlWriter;
class ILogger;

/**
 * @brief Responsible for reading and writing TRAC files.
 *
 * Depends on XmlReader, XmlWriter and ILogger injected via constructor.
 * All XML mapping logic lives in TRACMapping.hpp.
 */
class TRACRepository {
public:
    TRACRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger);

    /**
     * @brief Reads a TRAC file from disk and returns the parsed struct.
     * @param filePath Absolute or relative path to the TRAC XML file.
     * @return The parsed TRAC struct, or std::nullopt if reading or parsing failed.
     */
    [[nodiscard]] std::optional<OniFile<TRAC::Root>> load(const std::string& filePath) const;

    /**
     * @brief Writes an TRAC file to disk using the path stored in the OniFile wrapper.
     * @param file The TRAC data and its associated file path.
     * @return True if the file was written successfully, false otherwise.
     */
    [[nodiscard]] bool save(const OniFile<TRAC::Root>& file) const;

private:
    const XmlReader& m_reader;
    const XmlWriter& m_writer;
    const ILogger&   m_logger;

    /**
     * @brief Parses a TRAC struct from a loaded XmlDocument.
     * @param document A successfully loaded XmlDocument.
     * @return The parsed TRAC struct, or std::nullopt if the structure is unexpected.
     */
    [[nodiscard]] std::optional<TRAC::Root> parseDocument(const XmlDocument& document) const;

    [[nodiscard]] static XmlDocument buildDocument(const TRAC::Root& root) ;
};

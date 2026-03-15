#pragma once

#include <optional>
#include <string>

#include "component/xml/XmlDocument.hpp"
#include "model/TRAC.hpp"

// Forward declarations
class XmlReader;
class XmlWriter;
class ILogger;

/**
 * @brief Responsible for reading and writing TRAC files.
 *
 * Depends on XmlReader, XmlWriter and ILogger injected via constructor.
 * Knows how to map between the TRAC struct and its XML representation.
 */
class TRACRepository {
public:
    TRACRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger);

    /**
     * @brief Reads a TRAC file from disk and returns the parsed struct.
     * @param filePath Absolute or relative path to the TRAC XML file.
     * @return The parsed TRAC struct, or std::nullopt if reading or parsing failed.
     */
    [[nodiscard]] std::optional<TRAC> load(const std::string& filePath) const;

    /**
     * @brief Writes a TRAC struct to disk.
     * @param trac The TRAC struct to serialize.
     * @param filePath Absolute or relative path to the destination file.
     * @return True if the file was written successfully, false otherwise.
     */
    [[nodiscard]] bool save(const TRAC& trac, const std::string& filePath) const;

private:
    const XmlReader& m_reader;
    const XmlWriter& m_writer;
    const ILogger& m_logger;

    /**
     * @brief Parses a TRAC struct from a loaded XmlDocument.
     * @param document A successfully loaded XmlDocument.
     * @return The parsed TRAC struct, or std::nullopt if the structure is unexpected.
     */
    [[nodiscard]] std::optional<TRAC> parseDocument(const XmlDocument& document) const;

    /**
     * @brief Serializes a TRAC struct into a new XmlDocument.
     * @param trac The TRAC struct to serialize.
     * @return A populated XmlDocument ready to be written to disk.
     */
    [[nodiscard]] static XmlDocument buildDocument(const TRAC& trac);
};

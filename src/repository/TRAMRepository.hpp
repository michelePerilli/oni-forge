#pragma once

#include <optional>
#include <string>

#include "component/xml/XmlDocument.hpp"
#include "model/OniFile.hpp"
#include "model/TRAM.hpp"

// Forward declarations
class XmlReader;
class XmlWriter;
class ILogger;

/**
 * @brief Responsible for reading and writing TRAM files.
 *
 * All XML mapping logic lives in TRAMMapping.hpp.
 */
class TRAMRepository {
public:
    TRAMRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger);

    [[nodiscard]] std::optional<OniFile<TRAM::Root>> load(const std::string& filePath) const;
    [[nodiscard]] bool save(const OniFile<TRAM::Root>& file) const;

private:
    const XmlReader& m_reader;
    const XmlWriter& m_writer;
    const ILogger&   m_logger;

    [[nodiscard]] std::optional<TRAM::Root> parseDocument(const XmlDocument& document) const;
    [[nodiscard]] XmlDocument               buildDocument(const TRAM::Root& root) const;
};

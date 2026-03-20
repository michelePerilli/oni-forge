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
 * @brief Responsible for reading and writing ONCC files.
 *
 * Depends on XmlReader, XmlWriter and ILogger injected via constructor.
 * All XML mapping logic lives in ONCCMapping.hpp.
 */
class ONCCRepository {
public:
    ONCCRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger);

    [[nodiscard]] std::optional<OniFile<ONCC::Root>> load(const std::string& filePath) const;
    [[nodiscard]] bool save(const OniFile<ONCC::Root>& file) const;

private:
    const XmlReader& m_reader;
    const XmlWriter& m_writer;
    const ILogger&   m_logger;

    [[nodiscard]] std::optional<ONCC::Root> parseDocument(const XmlDocument& document) const;
    [[nodiscard]] static XmlDocument               buildDocument(const ONCC::Root& root);
};
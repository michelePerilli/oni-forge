#include "repository/ONCCRepository.hpp"
#include "model/mapping/ONCCMapping.hpp"

#include "component/logger/ILogger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"

ONCCRepository::ONCCRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger)
    : m_reader(reader)
    , m_writer(writer)
    , m_logger(logger) {}

std::optional<OniFile<ONCC::Root>> ONCCRepository::load(const std::string& filePath) const {
    XmlDocument document;
    if (!m_reader.read(filePath, document)) return std::nullopt;

    auto root = parseDocument(document);
    if (!root) return std::nullopt;

    return OniFile<ONCC::Root>{ filePath, *root };
}

bool ONCCRepository::save(const OniFile<ONCC::Root>& file) const {
    XmlDocument document = buildDocument(file.data);
    return m_writer.write(document, file.path.string());
}

std::optional<ONCC::Root> ONCCRepository::parseDocument(const XmlDocument& document) const {
    const pugi::xml_node oni = document.getRawDocument().child("Oni");
    if (!oni) {
        m_logger.error("[ONCCRepository] Missing <Oni> root node.");
        return std::nullopt;
    }

    ONCC::Root root;
    for (const auto& f : onccRootFields)
        f.read(oni, root);

    m_logger.info("[ONCCRepository] Parsed ONCC: health=" + root.oncc.health);
    return root;
}

XmlDocument ONCCRepository::buildDocument(const ONCC::Root& root) {
    XmlDocument document;
    pugi::xml_document& doc = document.getRawDocument();

    pugi::xml_node decl = doc.append_child(pugi::node_declaration);
    decl.append_attribute("version")  = "1.0";
    decl.append_attribute("encoding") = "utf-8";

    pugi::xml_node oni = doc.append_child("Oni");
    for (const auto& f : onccRootFields)
        f.write(oni, root);

    document.markAsLoaded();
    return document;
}
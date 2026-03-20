#include "repository/TRACRepository.hpp"
#include "model/mapping/TRACMapping.hpp"

#include "component/logger/ILogger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"

TRACRepository::TRACRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger)
    : m_reader(reader)
    , m_writer(writer)
    , m_logger(logger) {}

std::optional<OniFile<TRAC::Root>> TRACRepository::load(const std::string& filePath) const {
    XmlDocument document;
    if (!m_reader.read(filePath, document)) return std::nullopt;

    const auto root = parseDocument(document);
    if (!root) return std::nullopt;

    return OniFile<TRAC::Root>{ filePath, *root };
}

bool TRACRepository::save(const OniFile<TRAC::Root>& file) const {
    const XmlDocument document = buildDocument(file.data);
    return m_writer.write(document, file.path.string());
}

std::optional<TRAC::Root> TRACRepository::parseDocument(const XmlDocument& document) const {
    const pugi::xml_node oni = document.getRawDocument().child("Oni");
    if (!oni) {
        m_logger.error("[TRACRepository] Missing <Oni> root node.");
        return std::nullopt;
    }

    TRAC::Root root;
    for (const auto& [xmlName, write, read] : tracRootFields)
        read(oni.child("TRAC"), root);

    m_logger.info("[TRACRepository] Parsed TRAC with " +
                  std::to_string(root.animations.size()) + " animations.");
    return root;
}

XmlDocument TRACRepository::buildDocument(const TRAC::Root& root) {
    XmlDocument document;
    pugi::xml_document& doc = document.getRawDocument();

    pugi::xml_node decl = doc.append_child(pugi::node_declaration);
    decl.append_attribute("version")  = "1.0";
    decl.append_attribute("encoding") = "utf-8";

    pugi::xml_node oni  = doc.append_child("Oni");
    pugi::xml_node trac = oni.append_child("TRAC");
    for (const auto& [xmlName, write, read] : tracRootFields)
        write(trac, root);

    document.markAsLoaded();
    return document;
}
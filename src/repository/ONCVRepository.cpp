#include "repository/ONCVRepository.hpp"
#include "model/mapping/ONCVMapping.hpp"

#include "component/logger/ILogger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"

ONCVRepository::ONCVRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger)
    : m_reader(reader)
      , m_writer(writer)
      , m_logger(logger) {
}

std::optional<OniFile<ONCV::Root>> ONCVRepository::load(const std::string& filePath) const {
    XmlDocument document;
    if (!m_reader.read(filePath, document)) return std::nullopt;

    const auto root = parseDocument(document);
    if (!root) return std::nullopt;

    return OniFile<ONCV::Root>{filePath, *root};
}

bool ONCVRepository::save(const OniFile<ONCV::Root>& file) const {
    const XmlDocument document = buildDocument(file.data);
    return m_writer.write(document, file.path.string());
}

std::optional<ONCV::Root> ONCVRepository::parseDocument(const XmlDocument& document) const {
    const pugi::xml_node oni = document.getRawDocument().child("Oni");
    if (!oni) {
        m_logger.error("[ONCVRepository] Missing <Oni> root node.");
        return std::nullopt;
    }

    ONCV::Root root;
    for (const auto& f: oncvRootFields)
        f.read(oni.child("ONCV"), root);

    m_logger.info("[ONCVRepository] Parsed ONCV: " + root.characterClass);
    return root;
}

XmlDocument ONCVRepository::buildDocument(const ONCV::Root& root) {
    XmlDocument         document;
    pugi::xml_document& doc = document.getRawDocument();

    pugi::xml_node decl               = doc.append_child(pugi::node_declaration);
    decl.append_attribute("version")  = "1.0";
    decl.append_attribute("encoding") = "utf-8";

    pugi::xml_node oni  = doc.append_child("Oni");
    pugi::xml_node oncv = oni.append_child("ONCV");
    for (const auto& [xmlName, write, read]: oncvRootFields)
        write(oncv, root);

    document.markAsLoaded();
    return document;
}

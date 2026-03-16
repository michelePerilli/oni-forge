#include "repository/ONCVRepository.hpp"

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

    if (!m_reader.read(filePath, document)) {
        return std::nullopt;
    }

    auto oncv = parseDocument(document);
    if (!oncv) return std::nullopt;

    return OniFile{filePath, *oncv};
}

bool ONCVRepository::save(const OniFile<ONCV::Root>& file) const {
    const XmlDocument document = buildDocument(file.data);
    return m_writer.write(document, file.path.string());
}

std::optional<ONCV::Root> ONCVRepository::parseDocument(const XmlDocument& document) const {
    const pugi::xml_node root = document.getRawDocument().child("Oni");
    if (!root) {
        m_logger.error("[ONCVRepository] Missing <Oni> root node.");
        return std::nullopt;
    }

    const pugi::xml_node oncvNode = root.child("ONCV");
    if (!oncvNode) {
        m_logger.error("[ONCVRepository] Missing <ONCV> node.");
        return std::nullopt;
    }

    ONCV::Root oncv;
    oncv.id = oncvNode.attribute("id").as_int();
    oncv.parentVariant = oncvNode.child_value("ParentVariant");
    oncv.characterClass = oncvNode.child_value("CharacterClass");
    oncv.characterClassHard = oncvNode.child_value("CharacterClassHard");

    m_logger.info("[ONCVRepository] Parsed ONCV: " + oncv.characterClass);
    return oncv;
}

XmlDocument ONCVRepository::buildDocument(const ONCV::Root& oncv) {
    XmlDocument document;
    pugi::xml_document& doc = document.getRawDocument();

    pugi::xml_node decl = doc.append_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "utf-8";

    pugi::xml_node root = doc.append_child("Oni");
    pugi::xml_node oncvNode = root.append_child("ONCV");
    oncvNode.append_attribute("id") = oncv.id;

    oncvNode.append_child("ParentVariant").text().set(oncv.parentVariant.c_str());
    oncvNode.append_child("CharacterClass").text().set(oncv.characterClass.c_str());
    oncvNode.append_child("CharacterClassHard").text().set(oncv.characterClassHard.c_str());

    document.markAsLoaded();
    return document;
}

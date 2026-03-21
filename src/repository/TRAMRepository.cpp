#include "repository/TRAMRepository.hpp"
#include "model/mapping/TRAMMapping.hpp"

#include "component/logger/ILogger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"

TRAMRepository::TRAMRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger)
    : m_reader(reader)
    , m_writer(writer)
    , m_logger(logger) {}

std::optional<OniFile<TRAM::Root>> TRAMRepository::load(const std::string& filePath) const {
    XmlDocument document;
    if (!m_reader.read(filePath, document)) return std::nullopt;

    auto root = parseDocument(document);
    if (!root) return std::nullopt;

    return OniFile<TRAM::Root>{ filePath, *root };
}

bool TRAMRepository::save(const OniFile<TRAM::Root>& file) const {
    XmlDocument document = buildDocument(file.data);
    return m_writer.write(document, file.path.string());
}

std::optional<TRAM::Root> TRAMRepository::parseDocument(const XmlDocument& document) const {
    const pugi::xml_node oni = document.getRawDocument().child("Oni");
    if (!oni) {
        m_logger.error("[TRAMRepository] Missing <Oni> root node.");
        return std::nullopt;
    }

    const pugi::xml_node animNode = oni.child("Animation");
    if (!animNode) {
        m_logger.error("[TRAMRepository] Missing <Animation> node.");
        return std::nullopt;
    }

    TRAM::Root root;
    for (const auto& f : tramRootFields)
        f.read(animNode, root);

    const bool hasImport = root.importPath.has_value();
    const bool hasData   = root.animationData.has_value();
    m_logger.info("[TRAMRepository] Parsed TRAM — " +
                  std::string(hasImport ? "DAE import" : "inline data") +
                  (hasData ? " with animation data" : ""));
    return root;
}

XmlDocument TRAMRepository::buildDocument(const TRAM::Root& root) const {
    XmlDocument document;
    pugi::xml_document& doc = document.getRawDocument();

    pugi::xml_node decl = doc.append_child(pugi::node_declaration);
    decl.append_attribute("version")  = "1.0";
    decl.append_attribute("encoding") = "utf-8";

    pugi::xml_node oni      = doc.append_child("Oni");
    pugi::xml_node animNode = oni.append_child("Animation");

    for (const auto& f : tramRootFields)
        f.write(animNode, root);

    document.markAsLoaded();
    return document;
}

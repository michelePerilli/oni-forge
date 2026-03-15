#include "repository/TRACRepository.hpp"

#include "component/logger/ILogger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"

TRACRepository::TRACRepository(const XmlReader& reader, const XmlWriter& writer, const ILogger& logger)
    : m_reader(reader)
      , m_writer(writer)
      , m_logger(logger) {
}

[[nodiscard]] std::optional<OniFile<TRAC::Root>> TRACRepository::load(const std::string& filePath) const {
    XmlDocument document;

    if (!m_reader.read(filePath, document)) {
        return std::nullopt;
    }

    auto trac = parseDocument(document);
    if (!trac) return std::nullopt;

    return OniFile{filePath, *trac};
}

bool TRACRepository::save(const OniFile<TRAC::Root>& file) const {
    const XmlDocument document = buildDocument(file.data);
    return m_writer.write(document, file.path.string());
}

std::optional<TRAC::Root> TRACRepository::parseDocument(const XmlDocument& document) const {
    const pugi::xml_node root = document.getRawDocument().child("Oni");
    if (!root) {
        m_logger.error("[TRACRepository] Missing <Oni> root node.");
        return std::nullopt;
    }

    const pugi::xml_node tracNode = root.child("TRAC");
    if (!tracNode) {
        m_logger.error("[TRACRepository] Missing <TRAC> node.");
        return std::nullopt;
    }

    TRAC::Root trac;
    trac.id = tracNode.attribute("id").as_int();
    trac.parentCollection = tracNode.child_value("ParentCollection");

    for (const pugi::xml_node& animNode: tracNode.child("Animations").children("TRACAnimation")) {
        TRAC::TRACAnimation animation;
        animation.weight = animNode.child("Weight").text().as_int();
        animation.animation = animNode.child_value("Animation");
        trac.animations.push_back(animation);
    }

    m_logger.info("[TRACRepository] Parsed TRAC with " +
                  std::to_string(trac.animations.size()) + " animations.");
    return trac;
}

XmlDocument TRACRepository::buildDocument(const TRAC::Root& trac) {
    XmlDocument document;
    pugi::xml_document& doc = document.getRawDocument();

    // XML declaration
    pugi::xml_node decl = doc.append_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "utf-8";

    pugi::xml_node root = doc.append_child("Oni");
    pugi::xml_node tracNode = root.append_child("TRAC");
    tracNode.append_attribute("id") = trac.id;

    tracNode.append_child("ParentCollection").text().set(trac.parentCollection.c_str());

    pugi::xml_node animationsNode = tracNode.append_child("Animations");
    for (const auto& [weight, animation]: trac.animations) {
        pugi::xml_node animNode = animationsNode.append_child("TRACAnimation");
        animNode.append_child("Weight").text().set(weight);
        animNode.append_child("Animation").text().set(animation.c_str());
    }
    document.markAsLoaded();
    return document;
}

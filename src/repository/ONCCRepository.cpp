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
    // Attempt to read the XML file from disk
    if (!m_reader.read(filePath, document)) {
        m_logger.error("Failed to read ONCC file: " + filePath);
        return std::nullopt;
    }

    // Parse the XML document into the ONCC model
    auto root = parseDocument(document);
    if (!root) {
        m_logger.error("Failed to parse ONCC document: " + filePath);
        return std::nullopt;
    }

    return OniFile<ONCC::Root>{ filePath, *root };
}

bool ONCCRepository::save(const OniFile<ONCC::Root>& file) const {
    // Convert the ONCC model back to an XML document
    XmlDocument document = buildDocument(file.data);
    
    // Write the XML document to disk
    if (!m_writer.write(document, file.path.string())) {
        m_logger.error("Failed to write ONCC file: " + file.path.string());
        return false;
    }
    
    return true;
}

std::optional<ONCC::Root> ONCCRepository::parseDocument(const XmlDocument& document) const {
    // Access the raw pugixml document
    const auto& doc = document.getRawDocument();
    const pugi::xml_node oni = doc.child("Oni");
    
    if (!oni) {
        m_logger.error("[ONCCRepository] Missing <Oni> root node.");
        return std::nullopt;
    }

    ONCC::Root root;
    // Use the mapping definition to read fields from XML to the struct
    for (const auto& f : onccRootFields) {
        f.read(oni, root);
    }

    return root;
}

XmlDocument ONCCRepository::buildDocument(const ONCC::Root& root) {
    XmlDocument document;
    pugi::xml_document& doc = document.getRawDocument();

    // Add standard XML declaration
    pugi::xml_node decl = doc.append_child(pugi::node_declaration);
    decl.append_attribute("version")  = "1.0";
    decl.append_attribute("encoding") = "utf-8";

    // Create the root <Oni> element
    pugi::xml_node oni = doc.append_child("Oni");
    
    // Use the mapping definition to write fields from the struct to XML
    for (const auto& f : onccRootFields) {
        f.write(oni, root);
    }

    document.markAsLoaded();
    return document;
}

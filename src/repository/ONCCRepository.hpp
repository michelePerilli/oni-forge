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
 * Knows how to map between the ONCC::Root struct and its XML representation.
 */
class ONCCRepository {
public:
    ONCCRepository(const XmlReader& reader, const XmlWriter& writer, ILogger& logger);

    /**
     * @brief Reads an ONCC file from disk and returns the parsed struct.
     * @param filePath Absolute or relative path to the ONCC XML file.
     * @return The parsed ONCC::Root, or std::nullopt if reading or parsing failed.
     */
    [[nodiscard]] std::optional<OniFile<ONCC::Root>> load(const std::string& filePath) const;

    /**
     * @brief Writes an ONCC file to disk using the path stored in the OniFile wrapper.
     * @param file The ONCC data and its associated file path.
     * @return True if the file was written successfully, false otherwise.
     */
    [[nodiscard]] bool save(const OniFile<ONCC::Root>& file) const;

private:
    const XmlReader& m_reader;
    const XmlWriter& m_writer;
    ILogger&         m_logger;

    [[nodiscard]] std::optional<ONCC::Root> parseDocument(const XmlDocument& document) const;
    [[nodiscard]] XmlDocument               buildDocument(const ONCC::Root& root) const;

    // --- parse helpers ---
    [[nodiscard]] ONCC::ONCC parseONCC(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::ONCP parseONCP(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::ONIA parseONIA(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::CBPM parseCBPM(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::CBPI parseCBPI(const pugi::xml_node& node) const;

    [[nodiscard]] ONCC::AirConstants        parseAirConstants(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::ShadowConstants     parseShadowConstants(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::JumpConstants       parseJumpConstants(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::CoverConstants      parseCoverConstants(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::AutoFreezeConstants parseAutoFreezeConstants(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::InventoryConstants  parseInventoryConstants(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::HurtSoundConstants  parseHurtSoundConstants(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::AIConstants         parseAIConstants(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::Targeting           parseTargeting(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::AIWeaponSkill       parseAIWeaponSkill(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::SoundConstants      parseSoundConstants(const pugi::xml_node& node) const;
    [[nodiscard]] ONCC::VisionConstants     parseVisionConstants(const pugi::xml_node& node) const;

    // --- build helpers ---
    void buildONCC(pugi::xml_node& parent, const ONCC::ONCC& oncc) const;
    void buildONCP(pugi::xml_node& parent, const ONCC::ONCP& oncp) const;
    void buildONIA(pugi::xml_node& parent, const ONCC::ONIA& onia) const;
    void buildCBPM(pugi::xml_node& parent, const ONCC::CBPM& cbpm) const;
    void buildCBPI(pugi::xml_node& parent, const ONCC::CBPI& cbpi) const;

    void buildAirConstants(pugi::xml_node& parent, const ONCC::AirConstants& data) const;
    void buildShadowConstants(pugi::xml_node& parent, const ONCC::ShadowConstants& data) const;
    void buildJumpConstants(pugi::xml_node& parent, const ONCC::JumpConstants& data) const;
    void buildCoverConstants(pugi::xml_node& parent, const ONCC::CoverConstants& data) const;
    void buildAutoFreezeConstants(pugi::xml_node& parent, const ONCC::AutoFreezeConstants& data) const;
    void buildInventoryConstants(pugi::xml_node& parent, const ONCC::InventoryConstants& data) const;
    void buildHurtSoundConstants(pugi::xml_node& parent, const ONCC::HurtSoundConstants& data) const;
    void buildAIConstants(pugi::xml_node& parent, const ONCC::AIConstants& data) const;
    void buildTargeting(pugi::xml_node& parent, const ONCC::Targeting& data) const;
    void buildAIWeaponSkill(pugi::xml_node& parent, const ONCC::AIWeaponSkill& data) const;
    void buildSoundConstants(pugi::xml_node& parent, const ONCC::SoundConstants& data) const;
    void buildVisionConstants(pugi::xml_node& parent, const ONCC::VisionConstants& data) const;
};
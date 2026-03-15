#include "repository/ONCCRepository.hpp"

#include "component/logger/ILogger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"

ONCCRepository::ONCCRepository(const XmlReader& reader, const XmlWriter& writer, ILogger& logger)
    : m_reader(reader)
      , m_writer(writer)
      , m_logger(logger) {
}

// ---------------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------------

std::optional<OniFile<ONCC::Root>> ONCCRepository::load(const std::string& filePath) const {
    XmlDocument document;
    if (!m_reader.read(filePath, document)) return std::nullopt;

    auto root = parseDocument(document);
    if (!root) return std::nullopt;

    return OniFile<ONCC::Root>{filePath, *root};
}

bool ONCCRepository::save(const OniFile<ONCC::Root>& file) const {
    XmlDocument document = buildDocument(file.data);
    return m_writer.write(document, file.path.string());
}

// ---------------------------------------------------------------------------
// Parse
// ---------------------------------------------------------------------------

std::optional<ONCC::Root> ONCCRepository::parseDocument(const XmlDocument& document) const {
    const pugi::xml_node oni = document.getRawDocument().child("Oni");
    if (!oni) {
        m_logger.error("[ONCCRepository] Missing <Oni> root node.");
        return std::nullopt;
    }

    ONCC::Root root;
    root.oncc = parseONCC(oni.child("ONCC"));
    root.oncp = parseONCP(oni.child("ONCP"));
    root.onia = parseONIA(oni.child("ONIA"));
    root.cbpm = parseCBPM(oni.child("CBPM"));
    root.cbpi = parseCBPI(oni.child("CBPI"));

    m_logger.info("[ONCCRepository] Parsed ONCC: health=" + root.oncc.health);
    return root;
}

ONCC::ONCC ONCCRepository::parseONCC(const pugi::xml_node& node) const {
    ONCC::ONCC oncc;
    oncc.id = node.attribute("id").as_string();
    oncc.airConstants = parseAirConstants(node.child("AirConstants"));
    oncc.shadowConstants = parseShadowConstants(node.child("ShadowConstants"));
    oncc.jumpConstants = parseJumpConstants(node.child("JumpConstants"));
    oncc.coverConstants = parseCoverConstants(node.child("CoverConstants"));
    oncc.autoFreezeConstants = parseAutoFreezeConstants(node.child("AutoFreezeConstants"));
    oncc.inventoryConstants = parseInventoryConstants(node.child("InventoryConstants"));
    oncc.hurtSoundConstants = parseHurtSoundConstants(node.child("HurtSoundConstants"));
    oncc.aiConstants = parseAIConstants(node.child("AIConstants"));

    for (const pugi::xml_node& f: node.child("LODConstants").children("Float"))
        oncc.lodConstants.push_back(f.text().as_string());

    // Optional string fields
    if (auto n = node.child("Variant")) oncc.variant = n.text().as_string();
    if (auto n = node.child("BodySet")) oncc.bodySet = n.text().as_string();
    if (auto n = node.child("BodyTextures")) oncc.bodyTextures = n.text().as_string();
    if (auto n = node.child("DeathParticle")) oncc.deathParticle = n.text().as_string();
    if (auto n = node.child("ImpactModifierName")) oncc.impactModifierName = n.text().as_string();
    if (auto n = node.child("FightModeTimer")) oncc.fightModeTimer = n.text().as_string();
    if (auto n = node.child("IdleAnimation1Timer")) oncc.idleAnimation1Timer = n.text().as_string();
    if (auto n = node.child("IdleAnimation2Timer")) oncc.idleAnimation2Timer = n.text().as_string();
    if (auto n = node.child("BodyMaterials")) oncc.bodyMaterialsRef = n.text().as_string();
    if (auto n = node.child("BodyImpacts")) oncc.bodyImpactsRef = n.text().as_string();

    // Particles ref
    if (auto n = node.child("Particles")) oncc.particlesRef = n.text().as_string();

    // Dual <Impacts>: ref (text only) vs inline list (element children)
    for (const pugi::xml_node& impNode: node.children("Impacts")) {
        // Check for actual element children, skipping whitespace nodes
        bool hasElementChildren = false;
        for (const pugi::xml_node& child: impNode.children()) {
            if (child.type() == pugi::node_element) {
                hasElementChildren = true;
                break;
            }
        }

        if (hasElementChildren) {
            for (const pugi::xml_node& imp: impNode.children("ONCCImpact"))
                oncc.impacts.push_back({imp.child_value("Name")});
        } else {
            const std::string val = impNode.text().as_string();
            if (!val.empty()) oncc.impactsRef = val;
        }
    }

    // Always present
    oncc.health = node.child("Health").text().as_string();
    oncc.feetBones = node.child_value("FeetBones");
    oncc.minBodySizeFactor = node.child("MinBodySizeFactor").text().as_string();
    oncc.maxBodySizeFactor = node.child("MaxBodySizeFactor").text().as_string();
    oncc.bossShieldProtectAmount = node.child("BossShieldProtectAmount").text().as_string();
    oncc.animations = node.child_value("Animations");
    oncc.aimingScreens = node.child_value("AimingScreens");
    oncc.aiRateOfFire = node.child("AIRateOfFire").text().as_string();
    oncc.deathDeleteDelay = node.child("DeathDeleteDelay").text().as_string();
    oncc.weaponHand = node.child("WeaponHand").text().as_string();
    oncc.hasDaodanPowers = node.child("HasDaodanPowers").text().as_string();
    oncc.hasSupershield = node.child("HasSupershield").text().as_string();
    oncc.cantTouchThis = node.child("CantTouchThis").text().as_string();

    for (const pugi::xml_node& f: node.child("DamageFactors").children("Float"))
        oncc.damageFactors.push_back(f.text().as_string());

    return oncc;
}

ONCC::ONCP ONCCRepository::parseONCP(const pugi::xml_node& node) const {
    ONCC::ONCP oncp;
    oncp.id = node.attribute("id").as_string();
    for (const pugi::xml_node& p: node.child("Particles").children("ONCPParticle"))
        oncp.particles.push_back({p.child_value("Name"), p.child_value("Type"), p.child_value("BodyPart")});
    return oncp;
}

ONCC::ONIA ONCCRepository::parseONIA(const pugi::xml_node& node) const {
    ONCC::ONIA onia;
    onia.id = node.attribute("id").as_string();
    for (const pugi::xml_node& i: node.child("Impacts").children("ONIAImpact"))
        onia.impacts.push_back({i.child_value("Name"), i.child_value("Type"), i.child_value("Modifier")});
    return onia;
}

ONCC::CBPM ONCCRepository::parseCBPM(const pugi::xml_node& node) const {
    ONCC::CBPM cbpm;
    cbpm.id = node.attribute("id").as_string();
    for (const pugi::xml_node& l: node.child("Materials").children("Link"))
        cbpm.materials.push_back(l.text().as_string());
    return cbpm;
}

ONCC::CBPI ONCCRepository::parseCBPI(const pugi::xml_node& node) const {
    ONCC::CBPI cbpi;
    cbpi.id = node.attribute("id").as_string();
    for (const pugi::xml_node& l: node.child("HitImpacts").children("Link"))
        cbpi.hitImpacts.push_back(l.text().as_string());
    for (const pugi::xml_node& l: node.child("BlockedImpacts").children("Link"))
        cbpi.blockedImpacts.push_back(l.text().as_string());
    for (const pugi::xml_node& l: node.child("KilledImpacts").children("Link"))
        cbpi.killedImpacts.push_back(l.text().as_string());
    return cbpi;
}

ONCC::AirConstants ONCCRepository::parseAirConstants(const pugi::xml_node& node) const {
    return {
        node.child("FallGravity").text().as_string(),
        node.child("JumpGravity").text().as_string(),
        node.child("JumpStartVelocity").text().as_string(),
        node.child("MaxVelocity").text().as_string(),
        node.child("JetpackAcceleration").text().as_string(),
        node.child("FramesFallGravity").text().as_string(),
        node.child("JetpackTimer").text().as_string(),
        node.child("MaxNoDamageFallingHeight").text().as_string(),
        node.child("MaxDamageFallingHeight").text().as_string()
    };
}

ONCC::ShadowConstants ONCCRepository::parseShadowConstants(const pugi::xml_node& node) const {
    return {
        node.child_value("Texture"),
        node.child("MaxHeight").text().as_string(),
        node.child("FadeHeight").text().as_string(),
        node.child("SizeMax").text().as_string(),
        node.child("SizeFade").text().as_string(),
        node.child("SizeMin").text().as_string(),
        node.child("AlphaMax").text().as_string(),
        node.child("AlphaFade").text().as_string()
    };
}

ONCC::JumpConstants ONCCRepository::parseJumpConstants(const pugi::xml_node& node) const {
    return {
        node.child("JumpDistance").text().as_string(),
        node.child("JumpHeight").text().as_string(),
        node.child("JumpDistanceSquares").text().as_string()
    };
}

ONCC::CoverConstants ONCCRepository::parseCoverConstants(const pugi::xml_node& node) const {
    return {
        node.child("RayIncrement").text().as_string(),
        node.child("RayMax").text().as_string(),
        node.child("RayAngle").text().as_string(),
        node.child("RayAngleMax").text().as_string()
    };
}

ONCC::AutoFreezeConstants ONCCRepository::parseAutoFreezeConstants(const pugi::xml_node& node) const {
    return {
        node.child("DistanceXZ").text().as_string(),
        node.child("DistanceY").text().as_string()
    };
}

ONCC::InventoryConstants ONCCRepository::parseInventoryConstants(const pugi::xml_node& node) const {
    return {node.child("HypoRegenerationRate").text().as_string()};
}

ONCC::HurtSoundConstants ONCCRepository::parseHurtSoundConstants(const pugi::xml_node& node) const {
    return {
        node.child("BasePercentage").text().as_string(),
        node.child("MaxPercentage").text().as_string(),
        node.child("PercentageThreshold").text().as_string(),
        node.child("Timer").text().as_string(),
        node.child("MinTimer").text().as_string(),
        node.child("MaxLight").text().as_string(),
        node.child("MaxMedium").text().as_string(),
        node.child("DeathChance").text().as_string(),
        node.child("VolumeTreshold").text().as_string(),
        node.child("MediumTreshold").text().as_string(),
        node.child("HeavyTreshold").text().as_string(),
        node.child("MinVolume").text().as_string(),
        node.child_value("LightSound"),
        node.child_value("MediumSound"),
        node.child_value("HeavySound"),
        node.child_value("DeathSound")
    };
}

ONCC::AIConstants ONCCRepository::parseAIConstants(const pugi::xml_node& node) const {
    ONCC::AIConstants ai;
    ai.flags = node.child_value("Flags");
    ai.rotationSpeed = node.child("RotationSpeed").text().as_string();
    ai.dazedMinFrames = node.child("DazedMinFrames").text().as_string();
    ai.dazedMaxFrames = node.child("DazedMaxFrames").text().as_string();
    ai.dodgeReactFrames = node.child("DodgeReactFrames").text().as_string();
    ai.dodgeTimeScale = node.child("DodgeTimeScale").text().as_string();
    ai.dodgeWeightScale = node.child("DodgeWeightScale").text().as_string();
    ai.targeting = parseTargeting(node.child("Targeting"));

    for (const pugi::xml_node& s: node.child("WeaponSkills").children("AIWeaponSkill"))
        ai.weaponSkills.push_back(parseAIWeaponSkill(s));

    if (auto n = node.child("DeadMakeSureDelay")) ai.deadMakeSureDelay = n.text().as_string();
    if (auto n = node.child("InvestigateBodyDelay")) ai.investigateBodyDelay = n.text().as_string();
    if (auto n = node.child("LostContactDelay")) ai.lostContactDelay = n.text().as_string();
    if (auto n = node.child("DeadTauntChance")) ai.deadTauntChance = n.text().as_string();
    if (auto n = node.child("GoForGunChance")) ai.goForGunChance = n.text().as_string();
    if (auto n = node.child("RunPickupChance")) ai.runPickupChance = n.text().as_string();
    if (auto n = node.child("CombatId")) ai.combatId = n.text().as_string();
    if (auto n = node.child("MeleeId")) ai.meleeId = n.text().as_string();
    if (auto n = node.child("SoundConstants")) ai.soundConstants = parseSoundConstants(n);
    if (auto n = node.child("VisionConstants")) ai.visionConstants = parseVisionConstants(n);
    if (auto n = node.child("HostileThreatDefiniteTimer")) ai.hostileThreatDefiniteTimer = n.text().as_string();
    if (auto n = node.child("HostileThreatStrongTimer")) ai.hostileThreatStrongTimer = n.text().as_string();
    if (auto n = node.child("HostileThreatWeakTimer")) ai.hostileThreatWeakTimer = n.text().as_string();
    if (auto n = node.child("FriendlyThreatDefiniteTimer")) ai.friendlyThreatDefiniteTimer = n.text().as_string();
    if (auto n = node.child("FriendlyThreatStrongTimer")) ai.friendlyThreatStrongTimer = n.text().as_string();
    if (auto n = node.child("FriendlyThreatWeakTimer")) ai.friendlyThreatWeakTimer = n.text().as_string();
    if (auto n = node.child("EarshotRadius")) ai.earshotRadius = n.text().as_string();

    return ai;
}

ONCC::Targeting ONCCRepository::parseTargeting(const pugi::xml_node& node) const {
    return {
        node.child("StartleMissAngle").text().as_string(),
        node.child("StartleMissDistance").text().as_string(),
        node.child("PredictAmount").text().as_string(),
        node.child("PredictPositionDelayFrames").text().as_string(),
        node.child("PredictDelayFrames").text().as_string(),
        node.child("PredictVelocityFrames").text().as_string(),
        node.child("PredictTrendFrames").text().as_string()
    };
}

ONCC::AIWeaponSkill ONCCRepository::parseAIWeaponSkill(const pugi::xml_node& node) const {
    return {
        node.child("RecoilCompensation").text().as_string(),
        node.child("BestAimingAngle").text().as_string(),
        node.child("ShotGroupError").text().as_string(),
        node.child("ShotGroupDecay").text().as_string(),
        node.child("ShootingInaccuracyMultiplier").text().as_string(),
        node.child("MinShotDelay").text().as_string(),
        node.child("MaxShotDelay").text().as_string()
    };
}

ONCC::SoundConstants ONCCRepository::parseSoundConstants(const pugi::xml_node& node) const {
    return {
        node.child("TauntProbability").text().as_string(),
        node.child("AlertProbability").text().as_string(),
        node.child("StartleProbability").text().as_string(),
        node.child("CheckBodyProbability").text().as_string(),
        node.child("PursueProbability").text().as_string(),
        node.child("CoverProbability").text().as_string(),
        node.child("SuperPunchProbability").text().as_string(),
        node.child("SuperKickProbability").text().as_string(),
        node.child("Super3Probability").text().as_string(),
        node.child("Super4Probability").text().as_string(),
        node.child_value("TauntSound"),
        node.child_value("AlertSound"),
        node.child_value("StartleSound"),
        node.child_value("CheckBodySound"),
        node.child_value("PursueSound"),
        node.child_value("CoverSound"),
        node.child_value("SuperPunchSound"),
        node.child_value("SuperKickSound"),
        node.child_value("Super3Sound"),
        node.child_value("Super4Sound")
    };
}

ONCC::VisionConstants ONCCRepository::parseVisionConstants(const pugi::xml_node& node) const {
    return {
        node.child("CentralDistance").text().as_string(),
        node.child("CentralMax").text().as_string(),
        node.child("CentralRange").text().as_string(),
        node.child("PeripheralDistance").text().as_string(),
        node.child("PeripheralMax").text().as_string(),
        node.child("PeripheralRange").text().as_string(),
        node.child("VerticalRange").text().as_string()
    };
}

// ---------------------------------------------------------------------------
// Build
// ---------------------------------------------------------------------------

XmlDocument ONCCRepository::buildDocument(const ONCC::Root& root) const {
    XmlDocument document;
    pugi::xml_document& doc = document.getRawDocument();

    pugi::xml_node decl = doc.append_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "utf-8";

    pugi::xml_node oni = doc.append_child("Oni");
    buildONCC(oni, root.oncc);
    buildONCP(oni, root.oncp);
    buildONIA(oni, root.onia);
    buildCBPM(oni, root.cbpm);
    buildCBPI(oni, root.cbpi);

    document.markAsLoaded();
    return document;
}

void ONCCRepository::buildONCC(pugi::xml_node& parent, const ONCC::ONCC& oncc) const {
    pugi::xml_node node = parent.append_child("ONCC");
    node.append_attribute("id") = oncc.id.c_str();

    buildAirConstants(node, oncc.airConstants);
    buildShadowConstants(node, oncc.shadowConstants);
    buildJumpConstants(node, oncc.jumpConstants);
    buildCoverConstants(node, oncc.coverConstants);
    buildAutoFreezeConstants(node, oncc.autoFreezeConstants);
    buildInventoryConstants(node, oncc.inventoryConstants);

    pugi::xml_node lod = node.append_child("LODConstants");
    for (const std::string& f: oncc.lodConstants)
        lod.append_child("Float").text().set(f.c_str());

    buildHurtSoundConstants(node, oncc.hurtSoundConstants);
    buildAIConstants(node, oncc.aiConstants);

    if (oncc.variant) node.append_child("Variant").text().set(oncc.variant->c_str());
    if (oncc.particlesRef) node.append_child("Particles").text().set(oncc.particlesRef->c_str());
    if (oncc.impactsRef) node.append_child("Impacts").text().set(oncc.impactsRef->c_str());
    if (oncc.impactModifierName) node.append_child("ImpactModifierName").text().set(oncc.impactModifierName->c_str());

    // Inline impacts list — always write the node, even if empty
    pugi::xml_node impactsNode = node.append_child("Impacts");
    for (const ONCC::ONCCImpact& imp: oncc.impacts)
        impactsNode.append_child("ONCCImpact").append_child("Name").text().set(imp.n.c_str());

    if (oncc.deathParticle) node.append_child("DeathParticle").text().set(oncc.deathParticle->c_str());
    if (oncc.bodySet) node.append_child("BodySet").text().set(oncc.bodySet->c_str());
    if (oncc.bodyTextures) node.append_child("BodyTextures").text().set(oncc.bodyTextures->c_str());
    if (oncc.bodyMaterialsRef) node.append_child("BodyMaterials").text().set(oncc.bodyMaterialsRef->c_str());
    if (oncc.bodyImpactsRef) node.append_child("BodyImpacts").text().set(oncc.bodyImpactsRef->c_str());
    if (oncc.fightModeTimer) node.append_child("FightModeTimer").text().set(oncc.fightModeTimer->c_str());
    if (oncc.idleAnimation1Timer)
        node.append_child("IdleAnimation1Timer").text().
                set(oncc.idleAnimation1Timer->c_str());
    if (oncc.idleAnimation2Timer)
        node.append_child("IdleAnimation2Timer").text().
                set(oncc.idleAnimation2Timer->c_str());

    node.append_child("Health").text().set(oncc.health.c_str());
    node.append_child("FeetBones").text().set(oncc.feetBones.c_str());
    node.append_child("MinBodySizeFactor").text().set(oncc.minBodySizeFactor.c_str());
    node.append_child("MaxBodySizeFactor").text().set(oncc.maxBodySizeFactor.c_str());

    pugi::xml_node dmg = node.append_child("DamageFactors");
    for (const std::string& f: oncc.damageFactors)
        dmg.append_child("Float").text().set(f.c_str());

    node.append_child("BossShieldProtectAmount").text().set(oncc.bossShieldProtectAmount.c_str());
    node.append_child("Animations").text().set(oncc.animations.c_str());
    node.append_child("AimingScreens").text().set(oncc.aimingScreens.c_str());
    node.append_child("AIRateOfFire").text().set(oncc.aiRateOfFire.c_str());
    node.append_child("DeathDeleteDelay").text().set(oncc.deathDeleteDelay.c_str());
    node.append_child("WeaponHand").text().set(oncc.weaponHand.c_str());
    node.append_child("HasDaodanPowers").text().set(oncc.hasDaodanPowers.c_str());
    node.append_child("HasSupershield").text().set(oncc.hasSupershield.c_str());
    node.append_child("CantTouchThis").text().set(oncc.cantTouchThis.c_str());
}

void ONCCRepository::buildONCP(pugi::xml_node& parent, const ONCC::ONCP& oncp) const {
    pugi::xml_node node = parent.append_child("ONCP");
    node.append_attribute("id") = oncp.id.c_str();
    pugi::xml_node particles = node.append_child("Particles");
    for (const ONCC::ONCPParticle& p: oncp.particles) {
        pugi::xml_node pNode = particles.append_child("ONCPParticle");
        pNode.append_child("Name").text().set(p.name.c_str());
        pNode.append_child("Type").text().set(p.type.c_str());
        pNode.append_child("BodyPart").text().set(p.bodyPart.c_str());
    }
}

void ONCCRepository::buildONIA(pugi::xml_node& parent, const ONCC::ONIA& onia) const {
    pugi::xml_node node = parent.append_child("ONIA");
    node.append_attribute("id") = onia.id.c_str();
    pugi::xml_node impacts = node.append_child("Impacts");
    for (const ONCC::ONIAImpact& i: onia.impacts) {
        pugi::xml_node iNode = impacts.append_child("ONIAImpact");
        iNode.append_child("Name").text().set(i.n.c_str());
        iNode.append_child("Type").text().set(i.type.c_str());
        iNode.append_child("Modifier").text().set(i.modifier.c_str());
    }
}

void ONCCRepository::buildCBPM(pugi::xml_node& parent, const ONCC::CBPM& cbpm) const {
    pugi::xml_node node = parent.append_child("CBPM");
    node.append_attribute("id") = cbpm.id.c_str();
    pugi::xml_node materials = node.append_child("Materials");
    for (const std::string& m: cbpm.materials)
        materials.append_child("Link").text().set(m.c_str());
}

void ONCCRepository::buildCBPI(pugi::xml_node& parent, const ONCC::CBPI& cbpi) const {
    pugi::xml_node node = parent.append_child("CBPI");
    node.append_attribute("id") = cbpi.id.c_str();

    pugi::xml_node hit = node.append_child("HitImpacts");
    for (const std::string& l: cbpi.hitImpacts)
        hit.append_child("Link").text().set(l.c_str());

    pugi::xml_node blocked = node.append_child("BlockedImpacts");
    for (const std::string& l: cbpi.blockedImpacts)
        blocked.append_child("Link").text().set(l.c_str());

    pugi::xml_node killed = node.append_child("KilledImpacts");
    for (const std::string& l: cbpi.killedImpacts)
        killed.append_child("Link").text().set(l.c_str());
}

void ONCCRepository::buildAirConstants(pugi::xml_node& parent, const ONCC::AirConstants& data) const {
    pugi::xml_node node = parent.append_child("AirConstants");
    node.append_child("FallGravity").text().set(data.fallGravity.c_str());
    node.append_child("JumpGravity").text().set(data.jumpGravity.c_str());
    node.append_child("JumpStartVelocity").text().set(data.jumpStartVelocity.c_str());
    node.append_child("MaxVelocity").text().set(data.maxVelocity.c_str());
    node.append_child("JetpackAcceleration").text().set(data.jetpackAcceleration.c_str());
    node.append_child("FramesFallGravity").text().set(data.framesFallGravity.c_str());
    node.append_child("JetpackTimer").text().set(data.jetpackTimer.c_str());
    node.append_child("MaxNoDamageFallingHeight").text().set(data.maxNoDamageFallingHeight.c_str());
    node.append_child("MaxDamageFallingHeight").text().set(data.maxDamageFallingHeight.c_str());
}

void ONCCRepository::buildShadowConstants(pugi::xml_node& parent, const ONCC::ShadowConstants& data) const {
    pugi::xml_node node = parent.append_child("ShadowConstants");
    node.append_child("Texture").text().set(data.texture.c_str());
    node.append_child("MaxHeight").text().set(data.maxHeight.c_str());
    node.append_child("FadeHeight").text().set(data.fadeHeight.c_str());
    node.append_child("SizeMax").text().set(data.sizeMax.c_str());
    node.append_child("SizeFade").text().set(data.sizeFade.c_str());
    node.append_child("SizeMin").text().set(data.sizeMin.c_str());
    node.append_child("AlphaMax").text().set(data.alphaMax.c_str());
    node.append_child("AlphaFade").text().set(data.alphaFade.c_str());
}

void ONCCRepository::buildJumpConstants(pugi::xml_node& parent, const ONCC::JumpConstants& data) const {
    pugi::xml_node node = parent.append_child("JumpConstants");
    node.append_child("JumpDistance").text().set(data.jumpDistance.c_str());
    node.append_child("JumpHeight").text().set(data.jumpHeight.c_str());
    node.append_child("JumpDistanceSquares").text().set(data.jumpDistanceSquares.c_str());
}

void ONCCRepository::buildCoverConstants(pugi::xml_node& parent, const ONCC::CoverConstants& data) const {
    pugi::xml_node node = parent.append_child("CoverConstants");
    node.append_child("RayIncrement").text().set(data.rayIncrement.c_str());
    node.append_child("RayMax").text().set(data.rayMax.c_str());
    node.append_child("RayAngle").text().set(data.rayAngle.c_str());
    node.append_child("RayAngleMax").text().set(data.rayAngleMax.c_str());
}

void ONCCRepository::buildAutoFreezeConstants(pugi::xml_node& parent, const ONCC::AutoFreezeConstants& data) const {
    pugi::xml_node node = parent.append_child("AutoFreezeConstants");
    node.append_child("DistanceXZ").text().set(data.distanceXZ.c_str());
    node.append_child("DistanceY").text().set(data.distanceY.c_str());
}

void ONCCRepository::buildInventoryConstants(pugi::xml_node& parent, const ONCC::InventoryConstants& data) const {
    pugi::xml_node node = parent.append_child("InventoryConstants");
    node.append_child("HypoRegenerationRate").text().set(data.hypoRegenerationRate.c_str());
}

void ONCCRepository::buildHurtSoundConstants(pugi::xml_node& parent, const ONCC::HurtSoundConstants& data) const {
    pugi::xml_node node = parent.append_child("HurtSoundConstants");
    node.append_child("BasePercentage").text().set(data.basePercentage.c_str());
    node.append_child("MaxPercentage").text().set(data.maxPercentage.c_str());
    node.append_child("PercentageThreshold").text().set(data.percentageThreshold.c_str());
    node.append_child("Timer").text().set(data.timer.c_str());
    node.append_child("MinTimer").text().set(data.minTimer.c_str());
    node.append_child("MaxLight").text().set(data.maxLight.c_str());
    node.append_child("MaxMedium").text().set(data.maxMedium.c_str());
    node.append_child("DeathChance").text().set(data.deathChance.c_str());
    node.append_child("VolumeTreshold").text().set(data.volumeTreshold.c_str());
    node.append_child("MediumTreshold").text().set(data.mediumTreshold.c_str());
    node.append_child("HeavyTreshold").text().set(data.heavyTreshold.c_str());
    node.append_child("MinVolume").text().set(data.minVolume.c_str());
    node.append_child("LightSound").text().set(data.lightSound.c_str());
    node.append_child("MediumSound").text().set(data.mediumSound.c_str());
    node.append_child("HeavySound").text().set(data.heavySound.c_str());
    node.append_child("DeathSound").text().set(data.deathSound.c_str());
}

void ONCCRepository::buildAIConstants(pugi::xml_node& parent, const ONCC::AIConstants& data) const {
    pugi::xml_node node = parent.append_child("AIConstants");
    node.append_child("Flags").text().set(data.flags.c_str());
    node.append_child("RotationSpeed").text().set(data.rotationSpeed.c_str());
    node.append_child("DazedMinFrames").text().set(data.dazedMinFrames.c_str());
    node.append_child("DazedMaxFrames").text().set(data.dazedMaxFrames.c_str());
    node.append_child("DodgeReactFrames").text().set(data.dodgeReactFrames.c_str());
    node.append_child("DodgeTimeScale").text().set(data.dodgeTimeScale.c_str());
    node.append_child("DodgeWeightScale").text().set(data.dodgeWeightScale.c_str());
    buildTargeting(node, data.targeting);

    pugi::xml_node skills = node.append_child("WeaponSkills");
    for (const ONCC::AIWeaponSkill& s: data.weaponSkills)
        buildAIWeaponSkill(skills, s);

    // Correct order: optional scalar fields come before SoundConstants/VisionConstants
    if (data.deadMakeSureDelay) node.append_child("DeadMakeSureDelay").text().set(data.deadMakeSureDelay->c_str());
    if (data.investigateBodyDelay)
        node.append_child("InvestigateBodyDelay").text().set(
            data.investigateBodyDelay->c_str());
    if (data.lostContactDelay) node.append_child("LostContactDelay").text().set(data.lostContactDelay->c_str());
    if (data.deadTauntChance) node.append_child("DeadTauntChance").text().set(data.deadTauntChance->c_str());
    if (data.goForGunChance) node.append_child("GoForGunChance").text().set(data.goForGunChance->c_str());
    if (data.runPickupChance) node.append_child("RunPickupChance").text().set(data.runPickupChance->c_str());
    if (data.combatId) node.append_child("CombatId").text().set(data.combatId->c_str());
    if (data.meleeId) node.append_child("MeleeId").text().set(data.meleeId->c_str());
    if (data.soundConstants) buildSoundConstants(node, *data.soundConstants);
    if (data.visionConstants) buildVisionConstants(node, *data.visionConstants);
    if (data.hostileThreatDefiniteTimer)
        node.append_child("HostileThreatDefiniteTimer").text().set(
            data.hostileThreatDefiniteTimer->c_str());
    if (data.hostileThreatStrongTimer)
        node.append_child("HostileThreatStrongTimer").text().set(
            data.hostileThreatStrongTimer->c_str());
    if (data.hostileThreatWeakTimer)
        node.append_child("HostileThreatWeakTimer").text().set(
            data.hostileThreatWeakTimer->c_str());
    if (data.friendlyThreatDefiniteTimer)
        node.append_child("FriendlyThreatDefiniteTimer").text().set(
            data.friendlyThreatDefiniteTimer->c_str());
    if (data.friendlyThreatStrongTimer)
        node.append_child("FriendlyThreatStrongTimer").text().set(
            data.friendlyThreatStrongTimer->c_str());
    if (data.friendlyThreatWeakTimer)
        node.append_child("FriendlyThreatWeakTimer").text().set(
            data.friendlyThreatWeakTimer->c_str());
    if (data.earshotRadius) node.append_child("EarshotRadius").text().set(data.earshotRadius->c_str());
}

void ONCCRepository::buildTargeting(pugi::xml_node& parent, const ONCC::Targeting& data) const {
    pugi::xml_node node = parent.append_child("Targeting");
    node.append_child("StartleMissAngle").text().set(data.startleMissAngle.c_str());
    node.append_child("StartleMissDistance").text().set(data.startleMissDistance.c_str());
    node.append_child("PredictAmount").text().set(data.predictAmount.c_str());
    node.append_child("PredictPositionDelayFrames").text().set(data.predictPositionDelayFrames.c_str());
    node.append_child("PredictDelayFrames").text().set(data.predictDelayFrames.c_str());
    node.append_child("PredictVelocityFrames").text().set(data.predictVelocityFrames.c_str());
    node.append_child("PredictTrendFrames").text().set(data.predictTrendFrames.c_str());
}

void ONCCRepository::buildAIWeaponSkill(pugi::xml_node& parent, const ONCC::AIWeaponSkill& data) const {
    pugi::xml_node node = parent.append_child("AIWeaponSkill");
    node.append_child("RecoilCompensation").text().set(data.recoilCompensation.c_str());
    node.append_child("BestAimingAngle").text().set(data.bestAimingAngle.c_str());
    node.append_child("ShotGroupError").text().set(data.shotGroupError.c_str());
    node.append_child("ShotGroupDecay").text().set(data.shotGroupDecay.c_str());
    node.append_child("ShootingInaccuracyMultiplier").text().set(data.shootingInaccuracyMultiplier.c_str());
    node.append_child("MinShotDelay").text().set(data.minShotDelay.c_str());
    node.append_child("MaxShotDelay").text().set(data.maxShotDelay.c_str());
}

void ONCCRepository::buildSoundConstants(pugi::xml_node& parent, const ONCC::SoundConstants& data) const {
    pugi::xml_node node = parent.append_child("SoundConstants");
    node.append_child("TauntProbability").text().set(data.tauntProbability.c_str());
    node.append_child("AlertProbability").text().set(data.alertProbability.c_str());
    node.append_child("StartleProbability").text().set(data.startleProbability.c_str());
    node.append_child("CheckBodyProbability").text().set(data.checkBodyProbability.c_str());
    node.append_child("PursueProbability").text().set(data.pursueProbability.c_str());
    node.append_child("CoverProbability").text().set(data.coverProbability.c_str());
    node.append_child("SuperPunchProbability").text().set(data.superPunchProbability.c_str());
    node.append_child("SuperKickProbability").text().set(data.superKickProbability.c_str());
    node.append_child("Super3Probability").text().set(data.super3Probability.c_str());
    node.append_child("Super4Probability").text().set(data.super4Probability.c_str());
    node.append_child("TauntSound").text().set(data.tauntSound.c_str());
    node.append_child("AlertSound").text().set(data.alertSound.c_str());
    node.append_child("StartleSound").text().set(data.startleSound.c_str());
    node.append_child("CheckBodySound").text().set(data.checkBodySound.c_str());
    node.append_child("PursueSound").text().set(data.pursueSound.c_str());
    node.append_child("CoverSound").text().set(data.coverSound.c_str());
    node.append_child("SuperPunchSound").text().set(data.superPunchSound.c_str());
    node.append_child("SuperKickSound").text().set(data.superKickSound.c_str());
    node.append_child("Super3Sound").text().set(data.super3Sound.c_str());
    node.append_child("Super4Sound").text().set(data.super4Sound.c_str());
}

void ONCCRepository::buildVisionConstants(pugi::xml_node& parent, const ONCC::VisionConstants& data) const {
    pugi::xml_node node = parent.append_child("VisionConstants");
    node.append_child("CentralDistance").text().set(data.centralDistance.c_str());
    node.append_child("PeripheralDistance").text().set(data.peripheralDistance.c_str());
    node.append_child("VerticalRange").text().set(data.verticalRange.c_str());
    node.append_child("CentralRange").text().set(data.centralRange.c_str());
    node.append_child("CentralMax").text().set(data.centralMax.c_str());
    node.append_child("PeripheralRange").text().set(data.peripheralRange.c_str());
    node.append_child("PeripheralMax").text().set(data.peripheralMax.c_str());
}

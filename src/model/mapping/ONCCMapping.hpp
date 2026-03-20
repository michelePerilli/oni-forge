#pragma once

#include "model/mapping/XmlField.hpp"
#include "model/ONCC.hpp"
#include <vector>

// ---------------------------------------------------------------------------
// Sub-struct field mappings
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<ONCC::AirConstants>> airConstantsFields = {
    STR_FIELD("FallGravity",              fallGravity),
    STR_FIELD("JumpGravity",              jumpGravity),
    STR_FIELD("JumpStartVelocity",        jumpStartVelocity),
    STR_FIELD("MaxVelocity",              maxVelocity),
    STR_FIELD("JetpackAcceleration",      jetpackAcceleration),
    STR_FIELD("FramesFallGravity",        framesFallGravity),
    STR_FIELD("JetpackTimer",             jetpackTimer),
    STR_FIELD("MaxNoDamageFallingHeight", maxNoDamageFallingHeight),
    STR_FIELD("MaxDamageFallingHeight",   maxDamageFallingHeight),
};

inline const std::vector<XmlField<ONCC::ShadowConstants>> shadowConstantsFields = {
    STR_FIELD("Texture",   texture),
    STR_FIELD("MaxHeight", maxHeight),
    STR_FIELD("FadeHeight",fadeHeight),
    STR_FIELD("SizeMax",   sizeMax),
    STR_FIELD("SizeFade",  sizeFade),
    STR_FIELD("SizeMin",   sizeMin),
    STR_FIELD("AlphaMax",  alphaMax),
    STR_FIELD("AlphaFade", alphaFade),
};

inline const std::vector<XmlField<ONCC::JumpConstants>> jumpConstantsFields = {
    STR_FIELD("JumpDistance",        jumpDistance),
    STR_FIELD("JumpHeight",          jumpHeight),
    STR_FIELD("JumpDistanceSquares", jumpDistanceSquares),
};

inline const std::vector<XmlField<ONCC::CoverConstants>> coverConstantsFields = {
    STR_FIELD("RayIncrement", rayIncrement),
    STR_FIELD("RayMax",       rayMax),
    STR_FIELD("RayAngle",     rayAngle),
    STR_FIELD("RayAngleMax",  rayAngleMax),
};

inline const std::vector<XmlField<ONCC::AutoFreezeConstants>> autoFreezeConstantsFields = {
    STR_FIELD("DistanceXZ", distanceXZ),
    STR_FIELD("DistanceY",  distanceY),
};

inline const std::vector<XmlField<ONCC::InventoryConstants>> inventoryConstantsFields = {
    STR_FIELD("HypoRegenerationRate", hypoRegenerationRate),
};

inline const std::vector<XmlField<ONCC::HurtSoundConstants>> hurtSoundConstantsFields = {
    STR_FIELD("BasePercentage",      basePercentage),
    STR_FIELD("MaxPercentage",       maxPercentage),
    STR_FIELD("PercentageThreshold", percentageThreshold),
    STR_FIELD("Timer",               timer),
    STR_FIELD("MinTimer",            minTimer),
    STR_FIELD("MaxLight",            maxLight),
    STR_FIELD("MaxMedium",           maxMedium),
    STR_FIELD("DeathChance",         deathChance),
    STR_FIELD("VolumeTreshold",      volumeTreshold),
    STR_FIELD("MediumTreshold",      mediumTreshold),
    STR_FIELD("HeavyTreshold",       heavyTreshold),
    STR_FIELD("MinVolume",           minVolume),
    STR_FIELD("LightSound",          lightSound),
    STR_FIELD("MediumSound",         mediumSound),
    STR_FIELD("HeavySound",          heavySound),
    STR_FIELD("DeathSound",          deathSound),
};

inline const std::vector<XmlField<ONCC::Targeting>> targetingFields = {
    STR_FIELD("StartleMissAngle",           startleMissAngle),
    STR_FIELD("StartleMissDistance",        startleMissDistance),
    STR_FIELD("PredictAmount",              predictAmount),
    STR_FIELD("PredictPositionDelayFrames", predictPositionDelayFrames),
    STR_FIELD("PredictDelayFrames",         predictDelayFrames),
    STR_FIELD("PredictVelocityFrames",      predictVelocityFrames),
    STR_FIELD("PredictTrendFrames",         predictTrendFrames),
};

inline const std::vector<XmlField<ONCC::AIWeaponSkill>> aiWeaponSkillFields = {
    STR_FIELD("RecoilCompensation",           recoilCompensation),
    STR_FIELD("BestAimingAngle",              bestAimingAngle),
    STR_FIELD("ShotGroupError",               shotGroupError),
    STR_FIELD("ShotGroupDecay",               shotGroupDecay),
    STR_FIELD("ShootingInaccuracyMultiplier", shootingInaccuracyMultiplier),
    STR_FIELD("MinShotDelay",                 minShotDelay),
    STR_FIELD("MaxShotDelay",                 maxShotDelay),
};

inline const std::vector<XmlField<ONCC::SoundConstants>> soundConstantsFields = {
    STR_FIELD("TauntProbability",      tauntProbability),
    STR_FIELD("AlertProbability",      alertProbability),
    STR_FIELD("StartleProbability",    startleProbability),
    STR_FIELD("CheckBodyProbability",  checkBodyProbability),
    STR_FIELD("PursueProbability",     pursueProbability),
    STR_FIELD("CoverProbability",      coverProbability),
    STR_FIELD("SuperPunchProbability", superPunchProbability),
    STR_FIELD("SuperKickProbability",  superKickProbability),
    STR_FIELD("Super3Probability",     super3Probability),
    STR_FIELD("Super4Probability",     super4Probability),
    STR_FIELD("TauntSound",            tauntSound),
    STR_FIELD("AlertSound",            alertSound),
    STR_FIELD("StartleSound",          startleSound),
    STR_FIELD("CheckBodySound",        checkBodySound),
    STR_FIELD("PursueSound",           pursueSound),
    STR_FIELD("CoverSound",            coverSound),
    STR_FIELD("SuperPunchSound",       superPunchSound),
    STR_FIELD("SuperKickSound",        superKickSound),
    STR_FIELD("Super3Sound",           super3Sound),
    STR_FIELD("Super4Sound",           super4Sound),
};

// Note: order is non-alphabetical and must be preserved
inline const std::vector<XmlField<ONCC::VisionConstants>> visionConstantsFields = {
    STR_FIELD("CentralDistance",    centralDistance),
    STR_FIELD("PeripheralDistance", peripheralDistance),
    STR_FIELD("VerticalRange",      verticalRange),
    STR_FIELD("CentralRange",       centralRange),
    STR_FIELD("CentralMax",         centralMax),
    STR_FIELD("PeripheralRange",    peripheralRange),
    STR_FIELD("PeripheralMax",      peripheralMax),
};

inline const std::vector<XmlField<ONCC::ONCPParticle>> oncpParticleFields = {
    STR_FIELD("Name",     name),
    STR_FIELD("Type",     type),
    STR_FIELD("BodyPart", bodyPart),
};

inline const std::vector<XmlField<ONCC::ONIAImpact>> oniaImpactFields = {
    STR_FIELD("Name",     name),
    STR_FIELD("Type",     type),
    STR_FIELD("Modifier", modifier),
};

// ---------------------------------------------------------------------------
// AIConstants
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<ONCC::AIConstants>> aiConstantsFields = {
    STR_FIELD("Flags",            flags),
    STR_FIELD("RotationSpeed",    rotationSpeed),
    STR_FIELD("DazedMinFrames",   dazedMinFrames),
    STR_FIELD("DazedMaxFrames",   dazedMaxFrames),
    STR_FIELD("DodgeReactFrames", dodgeReactFrames),
    STR_FIELD("DodgeTimeScale",   dodgeTimeScale),
    STR_FIELD("DodgeWeightScale", dodgeWeightScale),
    SUB_STRUCT_FIELD("Targeting",     targeting,     targetingFields),
    STRUCT_LIST_FIELD("WeaponSkills", "AIWeaponSkill", weaponSkills, ONCC::AIWeaponSkill, aiWeaponSkillFields),
    STR_FIELD("DeadMakeSureDelay",           deadMakeSureDelay),
    STR_FIELD("InvestigateBodyDelay",        investigateBodyDelay),
    STR_FIELD("LostContactDelay",            lostContactDelay),
    STR_FIELD("DeadTauntChance",             deadTauntChance),
    STR_FIELD("GoForGunChance",              goForGunChance),
    STR_FIELD("RunPickupChance",             runPickupChance),
    STR_FIELD("CombatId",                    combatId),
    STR_FIELD("MeleeId",                     meleeId),
    SUB_STRUCT_FIELD("SoundConstants",  soundConstants,  soundConstantsFields),
    SUB_STRUCT_FIELD("VisionConstants", visionConstants, visionConstantsFields),
    STR_FIELD("HostileThreatDefiniteTimer",  hostileThreatDefiniteTimer),
    STR_FIELD("HostileThreatStrongTimer",    hostileThreatStrongTimer),
    STR_FIELD("HostileThreatWeakTimer",      hostileThreatWeakTimer),
    STR_FIELD("FriendlyThreatDefiniteTimer", friendlyThreatDefiniteTimer),
    STR_FIELD("FriendlyThreatStrongTimer",   friendlyThreatStrongTimer),
    STR_FIELD("FriendlyThreatWeakTimer",     friendlyThreatWeakTimer),
    STR_FIELD("EarshotRadius",               earshotRadius),
};

// ---------------------------------------------------------------------------
// ONCC::ONCC
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<ONCC::ONCC>> onccFields = {
    ATTR_FIELD("id",                  id),
    SUB_STRUCT_FIELD("AirConstants",        airConstants,        airConstantsFields),
    SUB_STRUCT_FIELD("ShadowConstants",     shadowConstants,     shadowConstantsFields),
    SUB_STRUCT_FIELD("JumpConstants",       jumpConstants,       jumpConstantsFields),
    SUB_STRUCT_FIELD("CoverConstants",      coverConstants,      coverConstantsFields),
    SUB_STRUCT_FIELD("AutoFreezeConstants", autoFreezeConstants, autoFreezeConstantsFields),
    SUB_STRUCT_FIELD("InventoryConstants",  inventoryConstants,  inventoryConstantsFields),
    FLOAT_VEC_FIELD("LODConstants",         lodConstants),
    SUB_STRUCT_FIELD("HurtSoundConstants",  hurtSoundConstants,  hurtSoundConstantsFields),
    SUB_STRUCT_FIELD("AIConstants",         aiConstants,         aiConstantsFields),
    STR_FIELD("Variant",                    variant),
    STR_FIELD("Particles",                  particlesRef),
    STR_FIELD("Impacts",                    impactsRef),
    STR_FIELD("ImpactModifierName",         impactModifierName),
    DUAL_IMPACTS_FIELD(impactsRef,          impacts),
    STR_FIELD("DeathParticle",              deathParticle),
    STR_FIELD("BodySet",                    bodySet),
    STR_FIELD("BodyTextures",               bodyTextures),
    STR_FIELD("BodyMaterials",              bodyMaterialsRef),
    STR_FIELD("BodyImpacts",               bodyImpactsRef),
    STR_FIELD("FightModeTimer",             fightModeTimer),
    STR_FIELD("IdleAnimation1Timer",        idleAnimation1Timer),
    STR_FIELD("IdleAnimation2Timer",        idleAnimation2Timer),
    STR_FIELD("Health",                     health),
    STR_FIELD("FeetBones",                  feetBones),
    STR_FIELD("MinBodySizeFactor",          minBodySizeFactor),
    STR_FIELD("MaxBodySizeFactor",          maxBodySizeFactor),
    FLOAT_VEC_FIELD("DamageFactors",        damageFactors),
    STR_FIELD("BossShieldProtectAmount",    bossShieldProtectAmount),
    STR_FIELD("Animations",                 animations),
    STR_FIELD("AimingScreens",              aimingScreens),
    STR_FIELD("AIRateOfFire",               aiRateOfFire),
    STR_FIELD("DeathDeleteDelay",           deathDeleteDelay),
    STR_FIELD("WeaponHand",                 weaponHand),
    STR_FIELD("HasDaodanPowers",            hasDaodanPowers),
    STR_FIELD("HasSupershield",             hasSupershield),
    STR_FIELD("CantTouchThis",              cantTouchThis),
};

// ---------------------------------------------------------------------------
// ONCP, ONIA, CBPM, CBPI
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<ONCC::ONCP>> oncpFields = {
    ATTR_FIELD("id", id),
    STRUCT_LIST_FIELD("Particles", "ONCPParticle", particles, ONCC::ONCPParticle, oncpParticleFields),
};

inline const std::vector<XmlField<ONCC::ONIA>> oniaFields = {
    ATTR_FIELD("id", id),
    STRUCT_LIST_FIELD("Impacts", "ONIAImpact", impacts, ONCC::ONIAImpact, oniaImpactFields),
};

inline const std::vector<XmlField<ONCC::CBPM>> cbpmFields = {
    ATTR_FIELD("id", id),
    LINK_LIST_FIELD("Materials", materials),
};

inline const std::vector<XmlField<ONCC::CBPI>> cbpiFields = {
    ATTR_FIELD("id", id),
    LINK_LIST_FIELD("HitImpacts",     hitImpacts),
    LINK_LIST_FIELD("BlockedImpacts", blockedImpacts),
    LINK_LIST_FIELD("KilledImpacts",  killedImpacts),
};

// ---------------------------------------------------------------------------
// ONCC::Root
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<ONCC::Root>> onccRootFields = {
    SUB_STRUCT_FIELD("ONCC", oncc, onccFields),
    SUB_STRUCT_FIELD("ONCP", oncp, oncpFields),
    SUB_STRUCT_FIELD("ONIA", onia, oniaFields),
    SUB_STRUCT_FIELD("CBPM", cbpm, cbpmFields),
    SUB_STRUCT_FIELD("CBPI", cbpi, cbpiFields),
};

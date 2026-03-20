#pragma once

#include <string>
#include <vector>

namespace ONCC {

// ---------------------------------------------------------------------------
// ONCC sub-structs
// ---------------------------------------------------------------------------

struct AirConstants {
    std::string fallGravity;
    std::string jumpGravity;
    std::string jumpStartVelocity;
    std::string maxVelocity;
    std::string jetpackAcceleration;
    std::string framesFallGravity;
    std::string jetpackTimer;
    std::string maxNoDamageFallingHeight;
    std::string maxDamageFallingHeight;
};

struct ShadowConstants {
    std::string texture;
    std::string maxHeight;
    std::string fadeHeight;
    std::string sizeMax;
    std::string sizeFade;
    std::string sizeMin;
    std::string alphaMax;
    std::string alphaFade;
};

struct JumpConstants {
    std::string jumpDistance;
    std::string jumpHeight;
    std::string jumpDistanceSquares;
};

struct CoverConstants {
    std::string rayIncrement;
    std::string rayMax;
    std::string rayAngle;
    std::string rayAngleMax;
};

struct AutoFreezeConstants {
    std::string distanceXZ;
    std::string distanceY;
};

struct InventoryConstants {
    std::string hypoRegenerationRate;
};

struct HurtSoundConstants {
    std::string basePercentage;
    std::string maxPercentage;
    std::string percentageThreshold;
    std::string timer;
    std::string minTimer;
    std::string maxLight;
    std::string maxMedium;
    std::string deathChance;
    std::string volumeTreshold;
    std::string mediumTreshold;
    std::string heavyTreshold;
    std::string minVolume;
    std::string lightSound;
    std::string mediumSound;
    std::string heavySound;
    std::string deathSound;
};

struct Targeting {
    std::string startleMissAngle;
    std::string startleMissDistance;
    std::string predictAmount;
    std::string predictPositionDelayFrames;
    std::string predictDelayFrames;
    std::string predictVelocityFrames;
    std::string predictTrendFrames;
};

struct AIWeaponSkill {
    std::string recoilCompensation;
    std::string bestAimingAngle;
    std::string shotGroupError;
    std::string shotGroupDecay;
    std::string shootingInaccuracyMultiplier;
    std::string minShotDelay;
    std::string maxShotDelay;
};

struct SoundConstants {
    std::string tauntProbability;
    std::string alertProbability;
    std::string startleProbability;
    std::string checkBodyProbability;
    std::string pursueProbability;
    std::string coverProbability;
    std::string superPunchProbability;
    std::string superKickProbability;
    std::string super3Probability;
    std::string super4Probability;
    std::string tauntSound;
    std::string alertSound;
    std::string startleSound;
    std::string checkBodySound;
    std::string pursueSound;
    std::string coverSound;
    std::string superPunchSound;
    std::string superKickSound;
    std::string super3Sound;
    std::string super4Sound;
};

struct VisionConstants {
    std::string centralDistance;
    std::string peripheralDistance;
    std::string verticalRange;
    std::string centralRange;
    std::string centralMax;
    std::string peripheralRange;
    std::string peripheralMax;
};

struct AIConstants {
    std::string flags;
    std::string rotationSpeed;
    std::string dazedMinFrames;
    std::string dazedMaxFrames;
    std::string dodgeReactFrames;
    std::string dodgeTimeScale;
    std::string dodgeWeightScale;
    Targeting                  targeting;
    std::vector<AIWeaponSkill> weaponSkills;
    std::string deadMakeSureDelay;
    std::string investigateBodyDelay;
    std::string lostContactDelay;
    std::string deadTauntChance;
    std::string goForGunChance;
    std::string runPickupChance;
    std::string combatId;
    std::string meleeId;
    SoundConstants  soundConstants;
    VisionConstants visionConstants;
    std::string hostileThreatDefiniteTimer;
    std::string hostileThreatStrongTimer;
    std::string hostileThreatWeakTimer;
    std::string friendlyThreatDefiniteTimer;
    std::string friendlyThreatStrongTimer;
    std::string friendlyThreatWeakTimer;
    std::string earshotRadius;
};

struct ONCCImpact {
    std::string name;
};

struct ONCC {
    std::string id;
    AirConstants        airConstants;
    ShadowConstants     shadowConstants;
    JumpConstants       jumpConstants;
    CoverConstants      coverConstants;
    AutoFreezeConstants autoFreezeConstants;
    InventoryConstants  inventoryConstants;
    std::vector<std::string> lodConstants;
    HurtSoundConstants  hurtSoundConstants;
    AIConstants         aiConstants;
    std::string variant;
    std::string particlesRef;
    std::string impactsRef;
    std::string impactModifierName;
    std::vector<ONCCImpact> impacts;
    std::string deathParticle;
    std::string bodySet;
    std::string bodyTextures;
    std::string bodyMaterialsRef;
    std::string bodyImpactsRef;
    std::string fightModeTimer;
    std::string idleAnimation1Timer;
    std::string idleAnimation2Timer;
    std::string health;
    std::string feetBones;
    std::string minBodySizeFactor;
    std::string maxBodySizeFactor;
    std::vector<std::string> damageFactors;
    std::string bossShieldProtectAmount;
    std::string animations;
    std::string aimingScreens;
    std::string aiRateOfFire;
    std::string deathDeleteDelay;
    std::string weaponHand;
    std::string hasDaodanPowers;
    std::string hasSupershield;
    std::string cantTouchThis;
};

// ---------------------------------------------------------------------------
// ONCP
// ---------------------------------------------------------------------------

struct ONCPParticle {
    std::string name;
    std::string type;
    std::string bodyPart;
};

struct ONCP {
    std::string id;
    std::vector<ONCPParticle> particles;
};

// ---------------------------------------------------------------------------
// ONIA
// ---------------------------------------------------------------------------

struct ONIAImpact {
    std::string name;
    std::string type;
    std::string modifier;
};

struct ONIA {
    std::string id;
    std::vector<ONIAImpact> impacts;
};

// ---------------------------------------------------------------------------
// CBPM
// ---------------------------------------------------------------------------

struct CBPM {
    std::string id;
    std::vector<std::string> materials;
};

// ---------------------------------------------------------------------------
// CBPI
// ---------------------------------------------------------------------------

struct CBPI {
    std::string id;
    std::vector<std::string> hitImpacts;
    std::vector<std::string> blockedImpacts;
    std::vector<std::string> killedImpacts;
};

// ---------------------------------------------------------------------------
// Root
// ---------------------------------------------------------------------------

/**
 * @brief Represents the full content of an ONCC file.
 *
 * An ONCC file contains multiple top-level blocks under <Oni>.
 * All blocks together define a character class.
 * Field names match the corresponding XML tag names.
 */
struct Root {
    ONCC oncc;
    ONCP oncp;
    ONIA onia;
    CBPM cbpm;
    CBPI cbpi;
};

} // namespace ONCC
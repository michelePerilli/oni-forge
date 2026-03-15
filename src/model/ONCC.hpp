#pragma once

#include <optional>
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
        std::string centralMax;
        std::string centralRange;
        std::string peripheralDistance;
        std::string peripheralMax;
        std::string peripheralRange;
        std::string verticalRange;
    };

    struct AIConstants {
        std::string flags;
        std::string rotationSpeed;
        std::string dazedMinFrames;
        std::string dazedMaxFrames;
        std::string dodgeReactFrames;
        std::string dodgeTimeScale;
        std::string dodgeWeightScale;
        Targeting targeting;
        std::vector<AIWeaponSkill> weaponSkills;

        // Optional fields — present only in some ONCC files
        std::optional<std::string> deadMakeSureDelay;
        std::optional<std::string> investigateBodyDelay;
        std::optional<std::string> lostContactDelay;
        std::optional<std::string> deadTauntChance;
        std::optional<std::string> goForGunChance;
        std::optional<std::string> runPickupChance;
        std::optional<std::string> combatId;
        std::optional<std::string> meleeId;
        std::optional<SoundConstants> soundConstants;
        std::optional<VisionConstants> visionConstants;
        std::optional<std::string> hostileThreatDefiniteTimer;
        std::optional<std::string> hostileThreatStrongTimer;
        std::optional<std::string> hostileThreatWeakTimer;
        std::optional<std::string> friendlyThreatDefiniteTimer;
        std::optional<std::string> friendlyThreatStrongTimer;
        std::optional<std::string> friendlyThreatWeakTimer;
        std::optional<std::string> earshotRadius;
    };

    struct ONCCImpact {
        std::string n;
    };

    struct ONCC {
        std::string id;

        AirConstants airConstants;
        ShadowConstants shadowConstants;
        JumpConstants jumpConstants;
        CoverConstants coverConstants;
        AutoFreezeConstants autoFreezeConstants;
        InventoryConstants inventoryConstants;
        std::vector<std::string> lodConstants;
        HurtSoundConstants hurtSoundConstants;
        AIConstants aiConstants;

        // Optional fields — present only in some ONCC files
        std::optional<std::string> variant;
        std::optional<std::string> particlesRef;
        std::optional<std::string> impactsRef;
        std::optional<std::string> impactModifierName;
        std::vector<ONCCImpact> impacts;
        std::optional<std::string> deathParticle;
        std::optional<std::string> bodySet;
        std::optional<std::string> bodyTextures;
        std::optional<std::string> bodyMaterialsRef;
        std::optional<std::string> bodyImpactsRef;
        std::optional<std::string> fightModeTimer;
        std::optional<std::string> idleAnimation1Timer;
        std::optional<std::string> idleAnimation2Timer;

        // Always present
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
        std::string n;
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
    // Root struct — represents the full content of an ONCC file
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
}

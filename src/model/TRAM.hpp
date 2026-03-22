#pragma once

#include <optional>
#include <string>
#include <vector>

namespace TRAM {
    // ---------------------------------------------------------------------------
    // Animation flags — space-separated string in XML (e.g. "Attack Atomic DontAim")
    // This list is used for future checkbox UI.
    // ---------------------------------------------------------------------------

    inline const std::vector<const char*> animFlags = {
        "RuntimeLoaded", "Invulnerable", "BlockHigh", "BlockLow",
        "Attack", "DropWeapon", "InAir", "Atomic", "NoTurn",
        "AttackForward", "AttackLeft", "AttackRight", "AttackBackward",
        "Overlay", "DontInterpolateVelocity", "ThrowSource", "ThrowTarget",
        "RealWorld", "DoAim", "DontAim", "CanPickup", "Aim360",
        "DisableShield", "NoAIPickup",
    };

    // ---------------------------------------------------------------------------
    // Direction values
    // ---------------------------------------------------------------------------

    inline const std::vector<const char*> animDirections = {
        "None", "Forward", "Backward", "Left", "Right",
    };

    // ---------------------------------------------------------------------------
    // Attack flags (inside <Attacks><Attack><Flags>) — different from top-level flags
    // ---------------------------------------------------------------------------

    inline const std::vector<const char*> attackFlags = {
        "Unblockable", "Low", "High", "HalfDamage",
    };

    // ---------------------------------------------------------------------------
    // Animation types
    // ---------------------------------------------------------------------------

    inline const std::vector<const char*> animTypes = {
        "None", "Anything", "Walk", "Run", "Slide", "Jump", "Stand",
        "StandingTurnLeft", "StandingTurnRight", "RunBackwards",
        "RunSidestepLeft", "RunSidestepRight", "Kick",
        "WalkSidestepLeft", "WalkSidestepRight", "WalkBackwards",
        "Crouch", "JumpForward", "JumpBackward",
        "Punch", "Block", "Land", "Fly",
        "KickForward", "KickLeft", "KickRight", "KickBack", "KickLow",
        "PunchForward", "PunchLeft", "PunchRight", "PunchBack", "PunchLow",
        "Kick2", "Kick3", "Punch2", "Punch3",
        "LandForward", "LandRight", "LandLeft", "LandBack",
        "PPK", "PunchHeavy", "KickHeavy", "KickForwardHeavy",
        "HitOverlay", "CrouchRun", "CrouchWalk",
        "CrouchRunBackwards", "CrouchWalkBackwards",
        "CrouchWalkSidestepLeft", "CrouchWalkSidestepRight",
        "RunKick", "RunPunch", "RunBackPunch", "RunBackKick",
        "SidestepLeftKick", "SidestepLeftPunch",
        "SidestepRightKick", "SidestepRightPunch",
        "Prone", "Flip",
        "HitHead", "HitBody", "HitFoot",
        "KnockdownHead", "KnockdownBody", "KnockdownFoot",
        "HitCrouch", "HitFallen",
        "HitHeadBehind", "HitBodyBehind", "HitFootBehind",
        "KnockdownHeadBehind", "KnockdownBodyBehind", "KnockdownFootBehind",
        "Idle", "Taunt",
        "Thrown1", "Thrown2", "Thrown3", "Thrown4", "Thrown5", "Thrown6",
        "Special1", "Special3",
        "ThrowForwardPunch", "ThrowForwardKick",
        "ThrowBackwardPunch", "ThrowBackwardKick",
        "RunThrowForwardPunch", "RunThrowBackwardPunch",
        "RunThrowForwardKick", "RunThrowBackwardKick",
        "Thrown7", "Thrown8", "Thrown10", "Thrown11", "Thrown12",
        "Sit", "StandSpecial", "Act", "Kick3Fw",
        "HitFootOuch", "HitJewels",
        "Thrown13", "Thrown15", "Thrown16",
        "PPKK", "PPKKK", "PPKKKK",
        "LandHard", "LandDead",
        "CrouchTurnLeft", "CrouchTurnRight",
        "CrouchForward", "CrouchBack", "CrouchLeft", "CrouchRight",
        "GetupKickBack",
        "AutopistolRecoil", "PhaseRifleRecoil", "PhaseStreamRecoil",
        "SuperballRecoil", "VandegrafRecoil", "ScramCannonRecoil",
        "MercuryBowRecoil", "ScreamerRecoil",
        "PickupObject", "PickupPistol", "PickupRifle",
        "Holster", "DrawPistol", "DrawRifle", "Punch4",
        "ReloadPistol", "ReloadPhaseRifle", "ReloadPhaseStream",
        "ReloadSuperball", "ReloadVandegraf", "ReloadScramCannon",
        "ReloadMercuryBow", "ReloadScreamer",
        "StartleLt", "StartleRt", "StartleBk", "StartleFw",
        "Console", "ConsoleWalk", "Stagger", "Watch",
        "ActNo", "ActYes", "ActTalk", "ActShrug", "ActShout", "ActGive",
        "RunStop", "WalkStop", "RunStart", "WalkStart",
        "RunBackwardsStart", "WalkBackwardsStart",
        "Stun", "StaggerBehind", "Blownup", "BlownupBehind",
        "OneStepStop", "RunSidestepLeftStart", "RunSidestepRightStart",
        "Powerup", "FallingFlail",
        "TeleportIn", "TeleportOut",
        "NinjaFireball", "NinjaInvisible", "PunchRifle",
        "PickupObjectMid", "PickupPistolMid", "PickupRifleMid",
        "Hail", "MuroThunderbolt",
    };

    // ---------------------------------------------------------------------------
    // Animation states
    // ---------------------------------------------------------------------------

    inline const std::vector<const char*> animStates = {
        "None", "Anything",
        "RunningLeftDown", "RunningRightDown", "Sliding",
        "WalkingLeftDown", "WalkingRightDown", "Standing",
        "RunStart", "RunAccel",
        "RunSidestepLeft", "RunSidestepRight", "RunSlide",
        "RunJump", "RunJumpLand", "RunBackStart",
        "RunningBackRightDown", "RunningBackLeftDown",
        "FallenBack", "Crouch",
        "RunningUpstairRightDown", "RunningUpstairLeftDown",
        "SidestepLeftLeftDown", "SidestepLeftRightDown",
        "SidestepRightLeftDown", "SidestepRightRightDown",
        "SidestepRightJump", "SidestepLeftJump",
        "JumpForward", "JumpUp", "RunBackSlide", "LieBack",
        "SsLtStart", "SsRtStart",
        "WalkingSidestepLeft", "CrouchWalk", "WalkingSidestepRight",
        "Flying", "Falling", "FlyingForward", "FallingForward",
        "FlyingBack", "FallingBack", "FlyingLeft", "FallingLeft",
        "FlyingRight", "FallingRight", "CrouchStart",
        "WalkingBackLeftDown", "WalkingBackRightDown", "FallenFront",
        "SidestepLeftStart", "SidestepRightStart",
        "Sit", "PunchLow", "StandSpecial", "Acting",
        "CrouchRunLeft", "CrouchRunRight",
        "CrouchRunBackLeft", "CrouchRunBackRight",
        "Blocking1", "Blocking2", "Blocking3", "CrouchBlocking1",
        "Gliding", "WatchIdle", "Stunned", "Powerup", "Thunderbolt",
    };

    // ---------------------------------------------------------------------------
    // Lookup sub-structs
    // ---------------------------------------------------------------------------

    struct Shortcut {
        std::string fromState;
        std::string length;
        std::string replaceAtomic;
    };

    struct Lookup {
        std::string           type;
        std::string           aimingType;
        std::string           fromState;
        std::string           toState;
        std::string           varient; // typo is intentional, matches XML tag
        std::string           firstLevel;
        std::vector<Shortcut> shortcuts;
    };

    // ---------------------------------------------------------------------------
    // Atomic / Invulnerable / Pause / Interpolation
    // ---------------------------------------------------------------------------

    struct FrameRange {
        std::string start;
        std::string end;
    };

    struct Pause {
        std::string hard;
        std::string soft;
    };

    struct Interpolation {
        std::string end;
        std::string max;
    };

    // ---------------------------------------------------------------------------
    // Overlay
    // ---------------------------------------------------------------------------

    struct Overlay {
        std::string usedBones;
        std::string replacedBones;
    };

    // ---------------------------------------------------------------------------
    // Particle
    // ---------------------------------------------------------------------------

    struct Particle {
        std::string start;
        std::string end;
        std::string bone;
        std::string name;
    };

    // ---------------------------------------------------------------------------
    // Attack / Extent
    // ---------------------------------------------------------------------------

    struct Extent {
        std::string angle;
        std::string length;
        std::string minY;
        std::string maxY;
    };

    struct Attack {
        std::string         start;
        std::string         end;
        std::string         bones;
        std::string         flags;
        std::string         knockback;
        std::string         hitPoints;
        std::string         hitType;
        std::string         hitLength;
        std::string         stunLength;
        std::string         staggerLength;
        std::vector<Extent> extents;
    };

    // ---------------------------------------------------------------------------
    // AnimationData — raw XML blobs, not user-editable.
    // frameCount is computed at load time and never serialized to XML.
    // ---------------------------------------------------------------------------

    struct AnimationData {
        std::string heights;
        std::string velocities;
        std::string rotations;
        std::string positionOffset;
        std::string positions;
        std::string attackRing;

        int frameCount = 0; // computed at load, NOT written to XML
    };

    // ---------------------------------------------------------------------------
    // Root
    // ---------------------------------------------------------------------------

    struct Root {
        std::optional<std::string> importPath;

        Lookup                   lookup;
        std::string              flags;
        FrameRange               atomic;
        FrameRange               invulnerable;
        Overlay                  overlay;
        std::vector<std::string> directAnimations;
        Pause                    pause;
        Interpolation            interpolation;
        std::string              finalRotation;
        std::string              direction;
        std::string              vocalization;
        std::string              actionFrame;
        std::string              impact;
        std::vector<Particle>    particles;
        std::string              motionBlur;
        std::string              footsteps;
        std::string              sounds;

        std::optional<AnimationData> animationData;

        std::string         throwSource;
        std::string         selfDamage;
        std::vector<Attack> attacks;
    };
} // namespace TRAM

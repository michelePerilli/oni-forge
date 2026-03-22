#pragma once

#include <optional>
#include <string>
#include <vector>

namespace TRAM {

// ---------------------------------------------------------------------------
// Lookup sub-structs
// ---------------------------------------------------------------------------

struct Shortcut {
    std::string fromState;
    std::string length;
    std::string replaceAtomic;
};

struct Lookup {
    std::string type;
    std::string aimingType;
    std::string fromState;
    std::string toState;
    std::string varient;  // note: typo is intentional, matches XML tag
    std::string firstLevel;
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
    std::string start;
    std::string end;
    std::string bones;
    std::string flags;
    std::string knockback;
    std::string hitPoints;
    std::string hitType;
    std::string hitLength;
    std::string stunLength;
    std::string staggerLength;
    std::vector<Extent> extents;
};

// ---------------------------------------------------------------------------
// AnimationData — raw XML blobs, not user-editable.
// Only present when the TRAM has inline animation data (no DAE import).
// frameCount is computed at load time and never serialized to XML.
// ---------------------------------------------------------------------------

struct AnimationData {
    std::string heights;
    std::string velocities;
    std::string rotations;
    std::string positionOffset;
    std::string positions;
    std::string attackRing;

    int frameCount = 0;  // computed at load, NOT written to XML
};

// ---------------------------------------------------------------------------
// Root
// ---------------------------------------------------------------------------

struct Root {
    std::optional<std::string> importPath;

    Lookup      lookup;
    std::string flags;
    FrameRange  atomic;
    FrameRange  invulnerable;
    Overlay     overlay;
    std::vector<std::string> directAnimations;
    Pause       pause;
    Interpolation interpolation;
    std::string finalRotation;
    std::string direction;
    std::string vocalization;
    std::string actionFrame;
    std::string impact;
    std::vector<Particle>    particles;
    std::string motionBlur;
    std::string footsteps;
    std::string sounds;

    std::optional<AnimationData> animationData;

    std::string throwSource;
    std::string selfDamage;
    std::vector<Attack> attacks;
};

} // namespace TRAM
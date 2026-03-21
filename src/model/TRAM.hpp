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
    std::string usedBones;    // text content of <UsedBones>
    std::string replacedBones; // text content of <ReplacedBones>
};

// ---------------------------------------------------------------------------
// Particle
// ---------------------------------------------------------------------------

struct Particle {
    std::string start;
    std::string end;
    std::string bone;
    std::string n;
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
// AnimationData — raw XML blobs, not user-editable
// Only present when the TRAM has inline animation data (no DAE import).
// Order mirrors the XML file order exactly.
// ---------------------------------------------------------------------------

struct AnimationData {
    std::string heights;       // raw XML content of <Heights>
    std::string velocities;    // raw XML content of <Velocities>
    std::string rotations;     // raw XML content of <Rotations>
    std::string positionOffset;// raw XML content of <PositionOffset>
    std::string positions;     // raw XML content of <Positions>
    std::string attackRing;    // raw XML content of <AttackRing>
};

// ---------------------------------------------------------------------------
// Root — top-level TRAM file content
// ---------------------------------------------------------------------------

/**
 * @brief Represents the full content of a TRAM file.
 *
 * The TRAM file has a single <Animation> block under <Oni>.
 * Editable fields are stored as typed members.
 * Raw animation data (Heights, Velocities, Rotations, Positions, AttackRing)
 * is stored as raw XML strings in AnimationData — it is not user-editable
 * and is swapped as a unit between TRAM files.
 *
 * importPath is set only when the file references an external .dae.
 * In that case, animationData will be empty.
 */
struct Root {
    std::optional<std::string> importPath;  // <Import Path="..."> if present

    Lookup      lookup;
    std::string flags;
    FrameRange  atomic;
    FrameRange  invulnerable;
    Overlay     overlay;
    std::vector<std::string> directAnimations; // <DirectAnimations><Link>...</Link>
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

    // Raw animation data — present only if no DAE import
    std::optional<AnimationData> animationData;

    std::string throwSource;
    std::string selfDamage;
    std::vector<Attack> attacks;
};

} // namespace TRAM
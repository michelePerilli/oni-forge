#pragma once

#include "model/mapping/XmlField.hpp"
#include "model/TRAM.hpp"
#include <sstream>
#include <vector>

// ---------------------------------------------------------------------------
// Helper: serialize/deserialize a single XML node to/from raw string
// ---------------------------------------------------------------------------

namespace TRAMMappingDetail {

inline std::string nodeToString(const pugi::xml_node& node) {
    std::ostringstream ss;
    for (const auto& child : node.children())
        child.print(ss, "    ", pugi::format_default);
    return ss.str();
}

inline void stringToChildren(pugi::xml_node& node, const std::string& raw) {
    if (!raw.empty()) {
        const std::string wrapped = "<r>" + raw + "</r>";
        pugi::xml_document tmp;
        if (tmp.load_string(wrapped.c_str()))
            for (const auto& child : tmp.child("r").children())
                node.append_copy(child);
    }
}

} // namespace TRAMMappingDetail

// ---------------------------------------------------------------------------
// Shortcut
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<TRAM::Shortcut>> shortcutFields = {
    STR_FIELD("FromState",     fromState),
    STR_FIELD("Length",        length),
    STR_FIELD("ReplaceAtomic", replaceAtomic),
};

// ---------------------------------------------------------------------------
// Lookup
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<TRAM::Lookup>> lookupFields = {
    STR_FIELD("Type",       type),
    STR_FIELD("AimingType", aimingType),
    STR_FIELD("FromState",  fromState),
    STR_FIELD("ToState",    toState),
    STR_FIELD("Varient",    varient),
    STR_FIELD("FirstLevel", firstLevel),
    STRUCT_LIST_FIELD("Shortcuts", "Shortcut", shortcuts, TRAM::Shortcut, shortcutFields),
};

// ---------------------------------------------------------------------------
// FrameRange (Atomic / Invulnerable)
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<TRAM::FrameRange>> frameRangeFields = {
    STR_FIELD("Start", start),
    STR_FIELD("End",   end),
};

// ---------------------------------------------------------------------------
// Pause
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<TRAM::Pause>> pauseFields = {
    STR_FIELD("Hard", hard),
    STR_FIELD("Soft", soft),
};

// ---------------------------------------------------------------------------
// Interpolation
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<TRAM::Interpolation>> interpolationFields = {
    STR_FIELD("End", end),
    STR_FIELD("Max", max),
};

// ---------------------------------------------------------------------------
// Overlay
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<TRAM::Overlay>> overlayFields = {
    STR_FIELD("UsedBones",     usedBones),
    STR_FIELD("ReplacedBones", replacedBones),
};

// ---------------------------------------------------------------------------
// Particle
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<TRAM::Particle>> particleFields = {
    STR_FIELD("Start", start),
    STR_FIELD("End",   end),
    STR_FIELD("Bone",  bone),
    STR_FIELD("n",     n),
};

// ---------------------------------------------------------------------------
// Extent
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<TRAM::Extent>> extentFields = {
    STR_FIELD("Angle",  angle),
    STR_FIELD("Length", length),
    STR_FIELD("MinY",   minY),
    STR_FIELD("MaxY",   maxY),
};

// ---------------------------------------------------------------------------
// Attack
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<TRAM::Attack>> attackFields = {
    STR_FIELD("Start",         start),
    STR_FIELD("End",           end),
    STR_FIELD("Bones",         bones),
    STR_FIELD("Flags",         flags),
    STR_FIELD("Knockback",     knockback),
    STR_FIELD("HitPoints",     hitPoints),
    STR_FIELD("HitType",       hitType),
    STR_FIELD("HitLength",     hitLength),
    STR_FIELD("StunLength",    stunLength),
    STR_FIELD("StaggerLength", staggerLength),
    STRUCT_LIST_FIELD("Extents", "Extent", extents, TRAM::Extent, extentFields),
};

// ---------------------------------------------------------------------------
// TRAM::Root — full mapping
//
// AnimationData fields (Heights, Velocities, Rotations, PositionOffset,
// Positions) are children of <Animation> directly, not wrapped in a tag.
// They are stored as raw XML strings in AnimationData and serialized back
// as-is. AttackRing follows Attacks, also stored in AnimationData.
//
// The read side populates animationData only if any of these nodes exist.
// The write side writes them only if animationData is present.
// ---------------------------------------------------------------------------

inline const std::vector<XmlField<TRAM::Root>> tramRootFields = {

    // <Import Path="..."> — present only if DAE reference
    OPT_ATTR_TAG_FIELD("Import", "Path", importPath),

    SUB_STRUCT_FIELD("Lookup",        lookup,       lookupFields),
    STR_FIELD("Flags",                flags),
    SUB_STRUCT_FIELD("Atomic",        atomic,       frameRangeFields),
    SUB_STRUCT_FIELD("Invulnerable",  invulnerable, frameRangeFields),
    SUB_STRUCT_FIELD("Overlay",       overlay,      overlayFields),
    LINK_LIST_FIELD("DirectAnimations", directAnimations),
    SUB_STRUCT_FIELD("Pause",         pause,        pauseFields),
    SUB_STRUCT_FIELD("Interpolation", interpolation,interpolationFields),
    STR_FIELD("FinalRotation",  finalRotation),
    STR_FIELD("Direction",      direction),
    STR_FIELD("Vocalization",   vocalization),
    STR_FIELD("ActionFrame",    actionFrame),
    STR_FIELD("Impact",         impact),
    STRUCT_LIST_FIELD("Particles", "Particle", particles, TRAM::Particle, particleFields),
    STR_FIELD("MotionBlur", motionBlur),
    STR_FIELD("Footsteps",  footsteps),
    STR_FIELD("Sounds",     sounds),

    // --- AnimationData block ---
    // Heights, Velocities, Rotations, PositionOffset, Positions are direct
    // children of <Animation> — stored raw and written back as-is.
    // Present only in files without a DAE import.
    { "AnimationData",
      [](pugi::xml_node& n, const TRAM::Root& o) {
          if (!o.animationData) return;
          const auto& d = *o.animationData;
          auto writeRaw = [&](const char* tag, const std::string& raw) {
              if (!raw.empty()) {
                  auto node = n.append_child(tag);
                  TRAMMappingDetail::stringToChildren(node, raw);
              }
          };
          writeRaw("Heights",        d.heights);
          writeRaw("Velocities",     d.velocities);
          writeRaw("Rotations",      d.rotations);
          writeRaw("PositionOffset", d.positionOffset);
          writeRaw("Positions",      d.positions);
      },
      [](const pugi::xml_node& n, TRAM::Root& o) {
          const bool hasData =
              n.child("Heights") || n.child("Velocities") ||
              n.child("Rotations") || n.child("Positions");
          if (!hasData) return;
          TRAM::AnimationData d;
          d.heights        = TRAMMappingDetail::nodeToString(n.child("Heights"));
          d.velocities     = TRAMMappingDetail::nodeToString(n.child("Velocities"));
          d.rotations      = TRAMMappingDetail::nodeToString(n.child("Rotations"));
          d.positionOffset = TRAMMappingDetail::nodeToString(n.child("PositionOffset"));
          d.positions      = TRAMMappingDetail::nodeToString(n.child("Positions"));
          o.animationData  = std::move(d);
      }
    },

    STR_FIELD("ThrowSource", throwSource),
    STR_FIELD("SelfDamage",  selfDamage),
    STRUCT_LIST_FIELD("Attacks", "Attack", attacks, TRAM::Attack, attackFields),

    // AttackRing — after Attacks, part of AnimationData
    { "AttackRing",
      [](pugi::xml_node& n, const TRAM::Root& o) {
          if (o.animationData && !o.animationData->attackRing.empty()) {
              auto node = n.append_child("AttackRing");
              TRAMMappingDetail::stringToChildren(node, o.animationData->attackRing);
          }
      },
      [](const pugi::xml_node& n, TRAM::Root& o) {
          if (const auto c = n.child("AttackRing")) {
              if (!o.animationData) o.animationData = TRAM::AnimationData{};
              o.animationData->attackRing = TRAMMappingDetail::nodeToString(c);
          }
      }
    },
};
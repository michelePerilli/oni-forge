#pragma once

#include "model/mapping/XmlField.hpp"
#include "model/ONCV.hpp"
#include <vector>

inline const std::vector<XmlField<ONCV::Root>> oncvRootFields = {
    ATTR_FIELD("id",                id),
    STR_FIELD("ParentVariant",      parentVariant),
    STR_FIELD("CharacterClass",     characterClass),
    STR_FIELD("CharacterClassHard", characterClassHard),
};

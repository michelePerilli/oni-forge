#pragma once

#include "model/mapping/XmlField.hpp"
#include "model/TRAC.hpp"
#include <vector>

inline const std::vector<XmlField<TRAC::TRACAnimation>> tracAnimationFields = {
    STR_FIELD("Weight",    weight),
    STR_FIELD("Animation", animation),
};

inline const std::vector<XmlField<TRAC::Root>> tracRootFields = {
    ATTR_FIELD("id",                id),
    STR_FIELD("ParentCollection",   parentCollection),
    STRUCT_LIST_FIELD("Animations", "TRACAnimation", animations, TRAC::TRACAnimation, tracAnimationFields),
};

#pragma once

#include <string>
#include <vector>
#include "model/ONCC.hpp"
#include "model/OniFile.hpp"
#include "service/ProjectCatalogService.hpp"
#include "service/VanillaCatalogService.hpp"

namespace OniForge::Validation {
    /**
     * @brief Represents the result of an ONCC impact validation.
     */
    struct OnccImpactValidationResult {
        ///< The filename stem of the validated ONCC.
        std::string onccStem;
        ///< List of impact names found in TRAMs but missing from the ONCC particle list.
        std::vector<std::string> unregisteredImpacts;
    };

    /**
     * @brief Validates that all impacts referenced by animations used by an ONCC are registered in its ONCP block.
     *
     * This function traverses the animation collection (TRAC) used by the character class,
     * inspects every animation metadata (TRAM) for impact effects, and ensures those
     * effects are registered in the ONCC's particle list (ONCP).
     *
     * @param onccFile The ONCC file to validate.
     * @param project The project catalog service (searched first).
     * @param vanilla The vanilla catalog service (searched if not in project).
     * @return An OnccImpactValidationResult containing the list of missing impact registrations.
     */
    OnccImpactValidationResult checkOnccImpacts(
        const OniFile<ONCC::Root>&   onccFile,
        const ProjectCatalogService& project,
        const VanillaCatalogService& vanilla
    );
}

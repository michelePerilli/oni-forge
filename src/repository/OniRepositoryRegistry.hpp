#pragma once

#include "repository/ONCCRepository.hpp"
#include "repository/ONCVRepository.hpp"
#include "repository/TRACRepository.hpp"
#include "repository/TRAMRepository.hpp"

/**
 * @brief Aggregates all repositories into a single injectable struct.
 *
 * This registry simplifies dependency injection by grouping all repository
 * instances. Injecting this struct instead of individual repositories prevents
 * constructor bloat in service classes. When a new file type is added, only this
 * struct needs to be updated, rather than modifying every service constructor that
 * requires data access.
 */
struct OniRepositoryRegistry {
    const ONCCRepository& oncc; ///< Repository for ONCC (Character Class) files.
    const ONCVRepository& oncv; ///< Repository for ONCV (Character Variant) files.
    const TRACRepository& trac; ///< Repository for TRAC (Animation Collection) files.
    const TRAMRepository& tram; ///< Repository for TRAM (Animation Metadata) files.
};

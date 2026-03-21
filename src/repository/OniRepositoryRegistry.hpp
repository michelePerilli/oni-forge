#pragma once

#include "repository/ONCCRepository.hpp"
#include "repository/ONCVRepository.hpp"
#include "repository/TRACRepository.hpp"
#include "repository/TRAMRepository.hpp"

/**
 * @brief Aggregates all repositories into a single injectable struct.
 *
 * Inject this struct instead of individual repositories to avoid
 * constructor bloat. When a new file type is added, only this struct
 * needs to be updated — not every service constructor.
 */
struct OniRepositoryRegistry {
    const ONCCRepository& oncc;
    const ONCVRepository& oncv;
    const TRACRepository& trac;
    const TRAMRepository& tram;
};
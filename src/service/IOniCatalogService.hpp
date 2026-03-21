#pragma once

#include <vector>

#include "model/ONCC.hpp"
#include "model/ONCV.hpp"
#include "model/OniFile.hpp"
#include "model/TRAC.hpp"
#include "model/TRAM.hpp"

/**
 * @brief Common interface for all catalog services.
 *
 * Provides read-only access to loaded Oni file collections.
 * Implemented by VanillaCatalogService, ModCatalogService and ProjectCatalogService.
 */
class IOniCatalogService {
public:
    virtual ~IOniCatalogService() = default;

    [[nodiscard]] virtual const std::vector<OniFile<ONCC::Root>>& getOnccFiles() const = 0;
    [[nodiscard]] virtual const std::vector<OniFile<ONCV::Root>>& getOncvFiles() const = 0;
    [[nodiscard]] virtual const std::vector<OniFile<TRAC::Root>>& getTracFiles() const = 0;
    [[nodiscard]] virtual const std::vector<OniFile<TRAM::Root>>& getTramFiles() const = 0;
};

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
 * Provides access to loaded Oni file collections.
 * Implemented by VanillaCatalogService and ProjectCatalogService.
 */
class IOniCatalogService {
public:
    virtual ~IOniCatalogService() = default;

    /**
     * @brief Gets all loaded Character Class (ONCC) files.
     * @return Const reference to the collection of ONCC files.
     */
    [[nodiscard]] virtual const std::vector<OniFile<ONCC::Root>>& getOnccFiles() const = 0;

    /**
     * @brief Gets all loaded Character Variant (ONCV) files.
     * @return Const reference to the collection of ONCV files.
     */
    [[nodiscard]] virtual const std::vector<OniFile<ONCV::Root>>& getOncvFiles() const = 0;

    /**
     * @brief Gets all loaded Animation Collection (TRAC) files.
     * @return Const reference to the collection of TRAC files.
     */
    [[nodiscard]] virtual const std::vector<OniFile<TRAC::Root>>& getTracFiles() const = 0;

    /**
     * @brief Gets all loaded Animation Metadata (TRAM) files.
     * @return Const reference to the collection of TRAM files.
     */
    [[nodiscard]] virtual const std::vector<OniFile<TRAM::Root>>& getTramFiles() const = 0;
};

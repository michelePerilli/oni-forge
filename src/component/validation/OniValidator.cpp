#include "component/validation/OniValidator.hpp"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace OniForge::Validation {
    /**
     * @brief Searches for an OniFile by its filename stem.
     *
     * Searches the project catalog first, then falls back to the vanilla catalog.
     * Uses std::string for compatibility with Windows paths (MinGW).
     *
     * @tparam T The type of the root data in the OniFile.
     * @param stem The filename stem to search for.
     * @param project The project catalog service.
     * @param vanilla The vanilla catalog service.
     * @param getFiles Pointer to the member function that returns the list of files of type T.
     * @return const OniFile<T>* A pointer to the found file, or nullptr if not found.
     */
    template<typename T>
    static const OniFile<T>* findFile(
        const std::string&                                    stem,
        const ProjectCatalogService&                          project,
        const VanillaCatalogService&                          vanilla,
        const std::vector<OniFile<T>>& (IOniCatalogService::* getFiles)() const
    ) {
        auto searchIn = [&](const IOniCatalogService& catalog) -> const OniFile<T>* {
            const auto& files = (catalog.*getFiles)();
            auto        it    = std::find_if(files.begin(), files.end(), [&](const auto& f) {
                return f.path.stem().string() == stem;
            });
            return (it != files.end()) ? &(*it) : nullptr;
        };

        if (const auto* found = searchIn(project)) return found;
        return searchIn(vanilla);
    }

    /**
     * @brief Builds a lookup map (stem -> OniFile) for O(1) searches.
     *
     * Resolves type conflicts between system paths and standard strings.
     * Project files override vanilla files with the same name.
     *
     * @tparam T The type of the root data in the OniFile.
     * @param project The project catalog service.
     * @param vanilla The vanilla catalog service.
     * @param getFiles Pointer to the member function that returns the list of files of type T.
     * @return std::unordered_map<std::string, const OniFile<T>*> A map for fast lookup.
     */
    template<typename T>
    static auto buildFileMap(
        const ProjectCatalogService&                          project,
        const VanillaCatalogService&                          vanilla,
        const std::vector<OniFile<T>>& (IOniCatalogService::* getFiles)() const
    ) {
        std::unordered_map<std::string, const OniFile<T>*> map;

        auto addToMap = [&](const IOniCatalogService& catalog) {
            const auto& files = (catalog.*getFiles)();
            for (const auto& file: files) {
                map[file.path.stem().string()] = &file;
            }
        };

        addToMap(vanilla);
        addToMap(project);
        return map;
    }

    OnccImpactValidationResult checkOnccImpacts(
        const OniFile<ONCC::Root>&   onccFile,
        const ProjectCatalogService& project,
        const VanillaCatalogService& vanilla
    ) {
        OnccImpactValidationResult result;
        result.onccStem = onccFile.path.stem().string();

        // 1. Collect impacts registered in the ONCC's ONCP block
        std::unordered_set<std::string> registeredParticles;
        for (const auto& [name, type, modifier]: onccFile.data.onia.impacts) {
            registeredParticles.insert(name);
        }

        // 2. Resolve the TRAC (Animation Collection) stem
        const std::string& tracStem = onccFile.data.oncc.animations;
        if (tracStem.empty()) return result;

        const auto* tracFile = findFile(tracStem, project, vanilla, &IOniCatalogService::getTracFiles);
        if (!tracFile) return result;

        // 3. Build TRAM map for O(1) animation metadata lookup
        auto tramMap = buildFileMap(project, vanilla, &IOniCatalogService::getTramFiles);

        // 4. Analyze animations in the TRAC
        std::unordered_set<std::string> missingImpactsSet;
        std::unordered_set<std::string> processedTrams;

        for (const auto& tracAnim: tracFile->data.animations) {
            const std::string& tramStem = tracAnim.animation;
            if (tramStem.empty()) continue;

            // Avoid redundant processing of the same TRAM
            if (!processedTrams.insert(tramStem).second) continue;

            // Fast lookup in the pre-built map
            auto it = tramMap.find(tramStem);
            if (it == tramMap.end()) continue;

            const std::string& impact = it->second->data.impact;
            if (impact.empty()) continue;

            // 5. Check if the TRAM's impact is registered in the ONCC particles
            if (!registeredParticles.contains(impact)) {
                missingImpactsSet.insert(impact);
            }
        }

        // 6. Populate result with unique missing impacts
        for (const auto& missing: missingImpactsSet) {
            result.unregisteredImpacts.push_back(missing);
        }

        return result;
    }
} // namespace OniForge::Validation

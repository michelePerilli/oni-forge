#pragma once

#include <filesystem>
#include <vector>
#include <mutex>
#include <memory>

#include "service/IOniCatalogService.hpp"
#include "FileWatch.hpp"

// Forward declarations
struct OniRepositoryRegistry;
class ILogger;

/**
 * @brief Editable catalog representing the current mod project.
 *
 * This service manages the loading, saving, and modification of mod files within a specific project folder.
 * It also facilitates the creation of new mod files by copying data from the read-only vanilla catalog.
 *
 * This class is thread-safe and integrates a file watcher that runs on a background thread.
 * When accessing file vectors from the render loop, you MUST lock the mutex provided by getMutex().
 *
 * Responsibilities:
 * - Loading project-specific ONCC, ONCV, TRAC, and TRAM files.
 * - Saving modifications back to the project folder.
 * - Cloning vanilla assets into the project for editing.
 * - Monitoring the project folder for file changes in real-time.
 */
class ProjectCatalogService final : public IOniCatalogService {
public:
    /**
     * @brief Constructs the ProjectCatalogService with necessary dependencies.
     *
     * @param repos Reference to the repository registry for file I/O operations.
     * @param vanilla Reference to the read-only vanilla catalog for sourcing base game data.
     * @param logger Reference to the logger for diagnostic output.
     */
    ProjectCatalogService(const OniRepositoryRegistry& repos,
                          const IOniCatalogService&    vanilla,
                          const ILogger&               logger);

    /**
     * @brief Loads all supported mod files from the specified project directory and starts the file watcher.
     *
     * Scans the directory for ONCC, ONCV, TRAC, and TRAM files and populates the internal collections.
     * Any existing file watcher is replaced with a new one for the specified path.
     *
     * @param folderPath The root path of the mod project.
     */
    void loadFromFolder(const std::filesystem::path& folderPath);

    /**
     * @brief Saves all loaded project files back to the specified output directory.
     *
     * Iterates through all internal file collections and serializes them to XML in the target folder.
     * Only files with a status other than Unmodified or Deleted are saved.
     * This operation is thread-safe.
     *
     * @param folderPath The destination path for saving files.
     */
    void saveToFolder(const std::filesystem::path& folderPath);

    /**
     * @brief Saves a single ONCC file to disk and resets its status to Unmodified.
     * @param file Reference to the ONCC file to save.
     */
    void saveFile(OniFile<ONCC::Root>& file);

    /**
     * @brief Saves a single ONCV file to disk and resets its status to Unmodified.
     * @param file Reference to the ONCV file to save.
     */
    void saveFile(OniFile<ONCV::Root>& file);

    /**
     * @brief Saves a single TRAC file to disk and resets its status to Unmodified.
     * @param file Reference to the TRAC file to save.
     */
    void saveFile(OniFile<TRAC::Root>& file);

    /**
     * @brief Saves a single TRAM file to disk and resets its status to Unmodified.
     * @param file Reference to the TRAM file to save.
     */
    void saveFile(OniFile<TRAM::Root>& file);

    /**
     * @brief Marks a file for deletion by setting its status to Deleted.
     * These operations are thread-safe.
     * @param file Reference to the file to mark.
     */
    void deleteFile(OniFile<ONCC::Root>& file);
    void deleteFile(OniFile<ONCV::Root>& file);
    void deleteFile(OniFile<TRAC::Root>& file);
    void deleteFile(OniFile<TRAM::Root>& file);

    /**
     * @brief Creates an editable copy of a vanilla ONCC file in the project.
     *
     * Looks up the file in the vanilla catalog by name. If found, a deep copy is added to the project's
     * collection with the 'Created' status. Thread-safe.
     *
     * @param name The filename stem (e.g., "ONCCbarabus") to search for in the vanilla catalog.
     * @return true If the file was found in vanilla and successfully added to the project.
     * @return false If the file was not found in the vanilla catalog.
     */
    [[nodiscard]] bool createOnccFromVanilla(const std::string& name);

    /**
     * @brief Creates an editable copy of a vanilla ONCV file in the project.
     *
     * @param name The filename stem to look up in the vanilla catalog.
     * @return true If the file was found in vanilla and successfully added to the project.
     */
    [[nodiscard]] bool createOncvFromVanilla(const std::string& name);

    /**
     * @brief Creates an editable copy of a vanilla TRAC file in the project.
     *
     * @param name The filename stem to look up in the vanilla catalog.
     * @return true If the file was found in vanilla and successfully added to the project.
     */
    [[nodiscard]] bool createTracFromVanilla(const std::string& name);

    /**
     * @brief Creates an editable copy of a vanilla TRAM file in the project.
     *
     * @param name The filename stem to look up in the vanilla catalog.
     * @return true If the file was found in vanilla and successfully added to the project.
     */
    [[nodiscard]] bool createTramFromVanilla(const std::string& name);

    /**
     * @brief Retrieves the collection of loaded ONCC (Character Class) files.
     * @note MUST be accessed while holding the mutex from getMutex().
     * @return A constant reference to the vector of ONCC files in the project.
     */
    [[nodiscard]] const std::vector<OniFile<ONCC::Root>>& getOnccFiles() const override;

    /**
     * @brief Retrieves the collection of loaded ONCV (Character Variant) files.
     * @note MUST be accessed while holding the mutex from getMutex().
     * @return A constant reference to the vector of ONCV files in the project.
     */
    [[nodiscard]] const std::vector<OniFile<ONCV::Root>>& getOncvFiles() const override;

    /**
     * @brief Retrieves the collection of loaded TRAC (Animation Collection) files.
     * @note MUST be accessed while holding the mutex from getMutex().
     * @return A constant reference to the vector of TRAC files in the project.
     */
    [[nodiscard]] const std::vector<OniFile<TRAC::Root>>& getTracFiles() const override;

    /**
     * @brief Retrieves the collection of loaded TRAM (Animation Metadata) files.
     * @note MUST be accessed while holding the mutex from getMutex().
     * @return A constant reference to the vector of TRAM files in the project.
     */
    [[nodiscard]] const std::vector<OniFile<TRAM::Root>>& getTramFiles() const override;

    /**
     * @brief Gets the mutex for thread-safe access to catalog collections.
     * @return Reference to the recursive mutex.
     */
    [[nodiscard]] std::recursive_mutex& getMutex() const override { return m_mutex; }

private:
    const OniRepositoryRegistry& m_repos;   ///< Registry containing repositories for file I/O.
    const IOniCatalogService&    m_vanilla; ///< Reference to the base game data service.
    const ILogger&               m_logger;  ///< Logger for error reporting and info messages.

    mutable std::recursive_mutex m_mutex; ///< Mutex to protect access to file vectors.

    std::vector<OniFile<ONCC::Root>> m_onccFiles;
    std::vector<OniFile<ONCV::Root>> m_oncvFiles;
    std::vector<OniFile<TRAC::Root>> m_tracFiles;
    std::vector<OniFile<TRAM::Root>> m_tramFiles;

    std::unique_ptr<filewatch::FileWatch<std::string>> m_fileWatcher; ///< Background file watcher.

    /**
     * @brief Helper to load ONCC files from the directory.
     * @param folderPath The directory to scan.
     */
    void loadOnccFiles(const std::filesystem::path& folderPath);

    /**
     * @brief Helper to load ONCV files from the directory.
     * @param folderPath The directory to scan.
     */
    void loadOncvFiles(const std::filesystem::path& folderPath);

    /**
     * @brief Helper to load TRAC files from the directory.
     * @param folderPath The directory to scan.
     */
    void loadTracFiles(const std::filesystem::path& folderPath);

    /**
     * @brief Helper to load TRAM files from the directory.
     * @param folderPath The directory to scan.
     */
    void loadTramFiles(const std::filesystem::path& folderPath);

    /**
     * @brief Handles file system events (add, modify, remove) for a specific file type.
     * Called from the background file watcher thread.
     * 
     * @tparam T The data type (ONCC::Root, etc.)
     * @tparam Repo The repository type.
     * @param files The vector of files to update.
     * @param repo The repository to use for loading.
     * @param path The absolute path to the file.
     * @param event The type of file system event.
     */
    template<typename T, typename Repo>
    void handleFileEvent(std::vector<OniFile<T>>& files, const Repo& repo, const std::filesystem::path& path, filewatch::Event event);
};

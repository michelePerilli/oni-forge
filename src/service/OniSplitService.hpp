#pragma once

#include <filesystem>
#include <functional>
#include <string>

/**
 * @brief Wraps OniSplit operations needed for "Try in ONI".
 *
 * This service orchestrates the complex multi-pass conversion pipeline required
 * to build Oni mods from source files (XML, DAE, images) into the game's
 * proprietary .oni and .dat formats. It ensures a clean build environment
 * and provides real-time feedback via callbacks.
 *
 * The full "Try in ONI" workflow involves these steps:
 *   1. Cleanup and Recreate temp folder
 *   2. Pass 1: Models (DAE -> TRBS .oni)
 *   3. Pass 2: Core XMLs (XML -> .oni)
 *   4. Pass 3: Images (XML -> .oni)
 *   5. Pass 4: Animations (Standard XML -> .oni & Blender XML -> .oni)
 *   6. Pass 5: Particles (XML -> .oni)
 *   7. packageDat()       — .oni files -> level0_alpha.dat
 *   8. copyDat()          — copy .dat to Oni GameDataFolder
 *   9. launchOni()        — launch Oni.exe
 *
 * Each step streams its output line-by-line via an onOutput callback
 * so the GUI can display progress in real time.
 */
class OniSplitService {
public:
    /// @brief Callback function type for streaming process output.
    using OutputCallback = std::function<void(const std::string& line)>;

    /**
     * @brief Constructs the service with paths to the tools and game.
     * @param oniSplitPath Absolute path to the OniSplit executable.
     * @param oniGamePath  Absolute path to the Oni game directory (containing GameDataFolder).
     */
    explicit OniSplitService(const std::string& oniSplitPath,
                             const std::string& oniGamePath);

    /**
     * @brief Runs the full "Try in ONI" pipeline.
     *
     * This orchestrates the multi-pass conversion, packaging, copying, and launching process.
     * It ensures a clean temporary build directory before starting the conversion passes.
     *
     * @param projectXmlPath  Root folder containing the mod's source XML, DAE, and image files.
     * @param tempOniPath     Temporary folder where intermediate .oni files will be written.
     * @param useSep          If true, uses -import:sep (Mac/PC Demo format) for packaging.
     *                        If false, uses -import:nosep (PC Retail format) for packaging.
     * @param onOutput        Callback function for streaming process output and progress messages.
     * @return true if all steps succeeded, false otherwise.
     */
    [[nodiscard]] bool tryInOni(const std::filesystem::path& projectXmlPath,
                                const std::filesystem::path& tempOniPath,
                                bool                         useSep,
                                const OutputCallback&        onOutput) const;

private:
    std::filesystem::path m_oniSplitPath; ///< Path to onisplit.exe.
    std::filesystem::path m_oniGamePath;  ///< Path to Oni installation folder.

    static constexpr std::string_view DAT_NAME         = "level0_alpha.dat"; ///< Target filename for the level package.
    static constexpr std::string_view GAME_DATA_FOLDER = "GameDataFolder";   ///< Subfolder in Oni where levels are stored.
    static constexpr std::string_view ONI_EXE          = "Oni.exe";          ///< Name of the Oni game executable.

    /**
     * @brief Executes a shell command and streams its output.
     * @param command The full command string to execute.
     * @param onOutput Callback for streaming output.
     * @return true if the process finished with exit code 0, false otherwise.
     */
    [[nodiscard]] bool runProcess(const std::string&    command,
                                  const OutputCallback& onOutput) const;

    /**
     * @brief Generic helper to convert various source files to .oni binary files using onisplit -create.
     * @param inputPath Base path to the source files (e.g., projectXmlPath/models).
     * @param outputPath Path where the output .oni files will be written (e.g., tempOniPath).
     * @param flags Additional flags for the onisplit -create command (e.g., ":trbs", "-blender").
     * @param wildcard Wildcard pattern to match input files (e.g., "*.xml", "*.dae").
     * @param callback Callback for streaming output.
     * @return true on success, false otherwise.
     */
    [[nodiscard]] bool createOni(const std::filesystem::path& inputPath,
                                 const std::filesystem::path& outputPath,
                                 const std::string&           flags,
                                 const std::string&           wildcard,
                                 const OutputCallback&        callback) const;

    /**
     * @brief Converts generic XML files to .oni binary files.
     * @param inputPath Path to the source XML files.
     * @param outputPath Path where the output .oni files will be written.
     * @param callback Callback for streaming output.
     * @return true on success, false otherwise.
     */
    [[nodiscard]] bool xmlToOni(const std::filesystem::path& inputPath,
                                const std::filesystem::path& outputPath,
                                const OutputCallback&        callback) const;

    /**
     * @brief Converts image XML files to .oni binary files.
     * @param inputPath Path to the source image XML files.
     * @param outputPath Path where the output .oni files will be written.
     * @param callback Callback for streaming output.
     * @return true on success, false otherwise.
     */
    [[nodiscard]] bool imageXmlToOni(const std::filesystem::path& inputPath,
                                     const std::filesystem::path& outputPath,
                                     const OutputCallback&        callback) const;

    /**
     * @brief Converts animation XML files to .oni binary files.
     * @param inputPath Path to the source animation XML files.
     * @param outputPath Path where the output .oni files will be written.
     * @param callback Callback for streaming output.
     * @param useBlender If true, the -blender flag is appended to the onisplit command for specialized processing.
     * @return true on success, false otherwise.
     */
    [[nodiscard]] bool animationXmlToOni(const std::filesystem::path& inputPath,
                                         const std::filesystem::path& outputPath,
                                         const OutputCallback&        callback,
                                         bool                         useBlender = false) const;

    /**
     * @brief Converts DAE (Collada) model files to TRBS .oni binary files.
     * @param inputPath Path to the source DAE files.
     * @param outputPath Path where the output .oni files will be written.
     * @param callback Callback for streaming output.
     * @param useBlender If true, the -blender flag is appended to the onisplit command.
     * @return true on success, false otherwise.
     */
    [[nodiscard]] bool trbsDaeToOni(const std::filesystem::path& inputPath,
                                    const std::filesystem::path& outputPath,
                                    const OutputCallback&        callback,
                                    bool                         useBlender = false) const;


    /**
     * @brief Packages .oni files into a .dat level file using onisplit -import.
     * @param oniPath Path containing the source .oni files.
     * @param datPath Path where the output .dat file will be created.
     * @param useSep  Controls the import format (-import:sep for Mac/PC Demo vs -import:nosep for PC Retail).
     * @param onOutput Callback for logging.
     * @return true on success, false otherwise.
     */
    [[nodiscard]] bool createDat(const std::filesystem::path& oniPath,
                                 const std::filesystem::path& datPath,
                                 bool                         useSep,
                                 const OutputCallback&        onOutput) const;

    /**
     * @brief Copies the generated .dat file to the Oni game's GameDataFolder.
     * @param datPath Path to the source .dat file.
     * @param onOutput Callback for streaming output.
     * @return true on success, false otherwise.
     */
    [[nodiscard]] bool copyDat(const std::filesystem::path& datPath,
                               const OutputCallback&        onOutput) const;

    /**
     * @brief Launches the Oni game executable.
     * @param onOutput Callback for streaming output.
     * @return true on success, false otherwise.
     */
    [[nodiscard]] bool launchOni(const OutputCallback& onOutput) const;
};

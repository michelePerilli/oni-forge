#pragma once

#include <filesystem>
#include <functional>
#include <string>

/**
 * @brief Wraps OniSplit operations needed for "Try in ONI".
 *
 * Flow:
 *   1. convertXmlToOni()  — XML → .oni files
 *   2. packageDat()       — .oni files → level0_alpha.dat
 *   3. copyDat()          — copy .dat to Oni GameDataFolder
 *   4. launchOni()        — launch Oni.exe
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
     * This orchestrates the conversion, packaging, copying, and launching process.
     *
     * @param projectXmlPath  Folder containing the mod XML files.
     * @param tempOniPath     Temp folder where .oni files will be written.
     * @param useSep          If true, uses -import:sep (Mac/PC Demo format).
     *                        If false, uses -import:nosep (PC Retail format).
     * @param onOutput        Callback function for streaming process output.
     * @return true if all steps succeeded.
     */
    bool tryInOni(const std::filesystem::path& projectXmlPath,
                  const std::filesystem::path& tempOniPath,
                  bool                         useSep,
                  const OutputCallback&        onOutput) const;

private:
    std::filesystem::path m_oniSplitPath;  ///< Path to onisplit.exe.
    std::filesystem::path m_oniGamePath;   ///< Path to Oni installation folder.

    static constexpr std::string_view DAT_NAME         = "level0_alpha.dat";
    static constexpr std::string_view GAME_DATA_FOLDER = "GameDataFolder";
    static constexpr std::string_view ONI_EXE          = "Oni.exe";

    /**
     * @brief Executes a shell command and streams its output.
     * @param command The command to execute.
     * @param onOutput Callback for streaming output.
     * @return true if the process finished with exit code 0.
     */
    bool runProcess(const std::string&    command,
                    const OutputCallback& onOutput) const;

    /**
     * @brief Converts XML files to .oni binary files.
     * @param xmlPath Path to the source XML files.
     * @param oniPath Path where the output .oni files will be written.
     * @param onOutput Callback for streaming output.
     * @return true on success.
     */
    bool convertXmlToOni(const std::filesystem::path& xmlPath,
                         const std::filesystem::path& oniPath,
                         const OutputCallback&        onOutput) const;

    /**
     * @brief Packages .oni files into a .dat level file.
     * @param oniPath Path containing the source .oni files.
     * @param datPath Path where the output .dat file will be created.
     * @param useSep  Controls the import format (-import:sep vs -import:nosep).
     * @param onOutput Callback for logging.
     * @return true on success.
     */
    bool packageDat(const std::filesystem::path& oniPath,
                    const std::filesystem::path& datPath,
                    bool                         useSep,
                    const OutputCallback&        onOutput) const;

    /**
     * @brief Copies the generated .dat file to the game directory.
     * @param datPath Path to the source .dat file.
     * @param onOutput Callback for streaming output.
     * @return true on success.
     */
    bool copyDat(const std::filesystem::path& datPath,
                 const OutputCallback&        onOutput) const;

    /**
     * @brief Launches the Oni game executable.
     * @param onOutput Callback for streaming output.
     * @return true on success.
     */
    bool launchOni(const OutputCallback& onOutput) const;
};

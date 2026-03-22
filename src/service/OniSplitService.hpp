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
    using OutputCallback = std::function<void(const std::string& line)>;

    explicit OniSplitService(const std::string& oniSplitPath,
                             const std::string& oniGamePath);

    /**
     * @brief Runs the full "Try in ONI" pipeline.
     * @param projectXmlPath  Folder containing the mod XML files.
     * @param tempOniPath     Temp folder where .oni files will be written.
     * @param onOutput        Called for each line of OniSplit output.
     * @return true if all steps succeeded.
     */
    bool tryInOni(const std::filesystem::path& projectXmlPath,
                  const std::filesystem::path& tempOniPath,
                  const OutputCallback&        onOutput) const;

private:
    std::filesystem::path m_oniSplitPath;  // path to onisplit.exe
    std::filesystem::path m_oniGamePath;   // path to Oni installation folder

    static constexpr std::string_view DAT_NAME         = "level0_alpha.dat";
    static constexpr std::string_view GAME_DATA_FOLDER = "GameDataFolder";
    static constexpr std::string_view ONI_EXE          = "Oni.exe";

    bool runProcess(const std::string&    command,
                    const OutputCallback& onOutput) const;

    bool convertXmlToOni(const std::filesystem::path& xmlPath,
                         const std::filesystem::path& oniPath,
                         const OutputCallback&        onOutput) const;

    bool packageDat(const std::filesystem::path& oniPath,
                    const std::filesystem::path& datPath,
                    const OutputCallback&        onOutput) const;

    bool copyDat(const std::filesystem::path& datPath,
                 const OutputCallback&        onOutput) const;

    bool launchOni(const OutputCallback& onOutput) const;
};
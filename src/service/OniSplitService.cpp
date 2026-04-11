#include "service/OniSplitService.hpp"

#include <array>
#include <cstdio>
#include <filesystem>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
  #include <unistd.h>
  #include <sys/types.h>
#endif

OniSplitService::OniSplitService(const std::string& oniSplitPath,
                                 const std::string& oniGamePath)
    : m_oniSplitPath(oniSplitPath)
      , m_oniGamePath(oniGamePath) {
}

// ---------------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------------

bool OniSplitService::tryInOni(const std::filesystem::path& projectXmlPath,
                               const std::filesystem::path& tempOniPath,
                               const bool                   useSep,
                               const OutputCallback&        onOutput) const {
    std::error_code ec;

    // Step 0: Cleanup
    onOutput("[OniSplit] Cleaning temp folder: " + tempOniPath.string());
    if (std::filesystem::exists(tempOniPath)) {
        std::filesystem::remove_all(tempOniPath, ec);
        if (ec) {
            onOutput("[OniSplit] Failed to clean temp folder: " + ec.message());
            return false;
        }
    }
    std::filesystem::create_directories(tempOniPath, ec);
    if (ec) {
        onOutput("[OniSplit] Failed to create temp folder: " + ec.message());
        return false;
    }

    // Pass 1: Models
    onOutput("[OniSplit] Pass 1/5 - Models (DAE -> TRBS)...");
    if (const auto modelPath = projectXmlPath / "model" / "blender"; std::filesystem::exists(modelPath)) {
        if (!trbsDaeToOni(modelPath, tempOniPath, onOutput, true)) {
            onOutput("[OniSplit] Errors occurred during model conversion in " + modelPath.string() + ". Skipping.");
        }
    } else {
        onOutput("[OniSplit] Skipping Pass 1: 'model' folder not found.");
    }
    if (const auto modelPath = projectXmlPath / "model" / "noblender"; std::filesystem::exists(modelPath)) {
        if (!trbsDaeToOni(modelPath, tempOniPath, onOutput, false)) {
            onOutput("[OniSplit] Errors occurred during model conversion in " + modelPath.string() + ". Skipping.");
        }
    } else {
        onOutput("[OniSplit] Skipping Pass 1: 'model' folder not found.");
    }

    // Pass 2: Core XMLs
    onOutput("[OniSplit] Pass 2/5 - Core XMLs...");
    if (!xmlToOni(projectXmlPath, tempOniPath, onOutput)) return false;

    // Pass 3: Images
    onOutput("[OniSplit] Pass 3/5 - Images...");
    if (const auto imagesPath = projectXmlPath / "images"; std::filesystem::exists(imagesPath)) {
        if (!imageXmlToOni(imagesPath, tempOniPath, onOutput)) return false;
    } else {
        onOutput("[OniSplit] Skipping Pass 3: 'images' folder not found.");
    }

    // Pass 4: Animations
    onOutput("[OniSplit] Pass 4/5 - Animations...");
    if (const auto animsPath = projectXmlPath / "animations" / "noblender";
        std::filesystem::exists(animsPath)) {
        onOutput("[OniSplit] Processing standard animations...");
        if (!animationXmlToOni(animsPath, tempOniPath, onOutput, false)) return false;
    }
    if (const auto animsPath = projectXmlPath / "animations" / "blender"; std::filesystem::exists(animsPath)) {
        onOutput("[OniSplit] Processing Blender animations...");
        if (!animationXmlToOni(animsPath, tempOniPath, onOutput, true)) return false;
    }

    // Pass 5: Particles
    onOutput("[OniSplit] Pass 5/5 - Particles...");
    if (const auto particlesPath = projectXmlPath / "particles"; std::filesystem::exists(particlesPath)) {
        if (!xmlToOni(particlesPath, tempOniPath, onOutput)) return false;
    } else {
        onOutput("[OniSplit] Skipping Pass 5: 'particles' folder not found.");
    }

    // Finalize
    const auto datPath = m_oniGamePath / GAME_DATA_FOLDER / DAT_NAME;

    onOutput("[OniSplit] Step: Packaging DAT...");
    if (!createDat(tempOniPath, datPath, useSep, onOutput)) {
        onOutput("[OniSplit] FAILED at packaging.");
        return false;
    }

    onOutput("[OniSplit] Step: Launching Oni...");
    if (!launchOni(onOutput)) {
        onOutput("[OniSplit] FAILED at launching.");
        return false;
    }

    onOutput("[OniSplit] Done!");
    return true;
}

// ---------------------------------------------------------------------------
// Private steps
// ---------------------------------------------------------------------------


bool OniSplitService::createOni(const std::filesystem::path& inputPath,
                                const std::filesystem::path& outputPath,
                                const std::string&           operation,
                                const std::string&           flags,
                                const std::string&           wildcard,
                                const OutputCallback&        callback) const {
    const std::string cmd =
            "\"" + m_oniSplitPath.string() + "\" " + operation +
            " \"" + outputPath.string() + "\" " + flags +
            " \"" + (inputPath / wildcard).string() + "\"";
    std::cout << cmd << std::endl;
    return runProcess(cmd, callback);
}

bool OniSplitService::createDat(const std::filesystem::path& oniPath,
                                const std::filesystem::path& datPath,
                                bool                         useSep,
                                const OutputCallback&        onOutput) const {
    const std::string cmd =
            "\"" + m_oniSplitPath.string() + "\" " + (useSep ? "-import:sep" : "-import:nosep") +
            " \"" + oniPath.string() + "\""
            " \"" + datPath.string() + "\"";
    return runProcess(cmd, onOutput);
}


bool OniSplitService::xmlToOni(const std::filesystem::path& inputPath,
                               const std::filesystem::path& outputPath,
                               const OutputCallback&        callback) const {
    return createOni(inputPath, outputPath, "-create", "", "*.xml", callback);
}

bool OniSplitService::imageXmlToOni(const std::filesystem::path& inputPath,
                                    const std::filesystem::path& outputPath,
                                    const OutputCallback&        callback) const {
    return createOni(inputPath, outputPath, "-create", "", "*.xml", callback);
}

bool OniSplitService::animationXmlToOni(const std::filesystem::path& inputPath,
                                        const std::filesystem::path& outputPath,
                                        const OutputCallback&        callback,
                                        const bool                   useBlender) const {
    return createOni(inputPath, outputPath, "-create", useBlender ? " -blender" : "", "*.xml", callback);
}

bool OniSplitService::trbsDaeToOni(const std::filesystem::path& inputPath,
                                   const std::filesystem::path& outputPath,
                                   const OutputCallback&        callback,
                                   const bool                   useBlender) const {
    return createOni(inputPath, outputPath, "-create:trbs", useBlender ? " -blender" : "", "*.dae", callback);
}

bool OniSplitService::launchOni(const OutputCallback& onOutput) const {
    const auto oniExe = m_oniGamePath / ONI_EXE;
    if (!std::filesystem::exists(oniExe)) {
        onOutput("[OniSplit] Oni.exe not found at: " + oniExe.string());
        return false;
    }

#ifdef _WIN32
    HINSTANCE result = ShellExecuteW(
        nullptr, L"open",
        oniExe.wstring().c_str(),
        nullptr,
        m_oniGamePath.wstring().c_str(),
        SW_SHOWNORMAL
    );
    if (reinterpret_cast<intptr_t>(result) <= 32) {
        onOutput("[OniSplit] Failed to launch Oni. Error: " +
                 std::to_string(reinterpret_cast<intptr_t>(result)));
        return false;
    }
#else
    const pid_t pid = fork();
    if (pid == 0) {
        chdir(m_oniGamePath.c_str());
        execl(oniExe.c_str(), oniExe.c_str(), nullptr);
        _exit(1);
    } else if (pid < 0) {
        onOutput("[OniSplit] Failed to fork process.");
        return false;
    }
#endif

    onOutput("[OniSplit] Oni launched.");
    return true;
}

// ---------------------------------------------------------------------------
// Process runner — captures stdout+stderr line by line
// ---------------------------------------------------------------------------

bool OniSplitService::runProcess(const std::string&    command,
                                 const OutputCallback& onOutput) const {
    onOutput("[CMD] " + command);

#ifdef _WIN32
    const std::string fullCmd = "cmd /c \"" + command + "\" 2>&1";
#else
    const std::string fullCmd = command + " 2>&1";
#endif

    FILE* pipe = popen(fullCmd.c_str(), "r");
    if (!pipe) {
        onOutput("[OniSplit] Failed to start process.");
        return false;
    }

    std::array<char, 512> buffer{};
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe))
        onOutput(std::string(buffer.data()));

    const int exitCode = pclose(pipe);
    if (exitCode != 0) {
        onOutput("[OniSplit] Process exited with code: " + std::to_string(exitCode));
        return false;
    }
    return true;
}

#include "service/OniSplitService.hpp"

#include <array>
#include <cstdio>
#include <filesystem>
#include <stdexcept>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <unistd.h>
  #include <sys/types.h>
#endif
OniSplitService::OniSplitService(const std::string& oniSplitPath,
                                 const std::string& oniGamePath)
    : m_oniSplitPath(oniSplitPath)
    , m_oniGamePath(oniGamePath) {}

// ---------------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------------

bool OniSplitService::tryInOni(const std::filesystem::path& projectXmlPath,
                               const std::filesystem::path& tempOniPath,
                               const OutputCallback&        onOutput) const {
    // Ensure temp folder exists
    std::error_code ec;
    std::filesystem::create_directories(tempOniPath, ec);
    if (ec) {
        onOutput("[OniSplit] Failed to create temp folder: " + ec.message());
        return false;
    }

    const auto datPath = tempOniPath / DAT_NAME;

    onOutput("[OniSplit] Step 1/4 — Converting XML to ONI...");
    if (!convertXmlToOni(projectXmlPath, tempOniPath, onOutput)) {
        onOutput("[OniSplit] FAILED at step 1.");
        return false;
    }

    onOutput("[OniSplit] Step 2/4 — Packaging DAT...");
    if (!packageDat(tempOniPath, datPath, onOutput)) {
        onOutput("[OniSplit] FAILED at step 2.");
        return false;
    }

    onOutput("[OniSplit] Step 3/4 — Copying DAT to Oni...");
    if (!copyDat(datPath, onOutput)) {
        onOutput("[OniSplit] FAILED at step 3.");
        return false;
    }

    onOutput("[OniSplit] Step 4/4 — Launching Oni...");
    if (!launchOni(onOutput)) {
        onOutput("[OniSplit] FAILED at step 4.");
        return false;
    }

    onOutput("[OniSplit] Done!");
    return true;
}

// ---------------------------------------------------------------------------
// Private steps
// ---------------------------------------------------------------------------

bool OniSplitService::convertXmlToOni(const std::filesystem::path& xmlPath,
                                      const std::filesystem::path& oniPath,
                                      const OutputCallback&        onOutput) const {
    const std::string cmd =
        "\"" + m_oniSplitPath.string() + "\" -create"
        " \"" + oniPath.string() + "\""
        " \"" + xmlPath.string() + "\\*\"";
    return runProcess(cmd, onOutput);
}

bool OniSplitService::packageDat(const std::filesystem::path& oniPath,
                                 const std::filesystem::path& datPath,
                                 const OutputCallback&        onOutput) const {
    const std::string cmd =
        "\"" + m_oniSplitPath.string() + "\" -import:nosep"
        " \"" + oniPath.string() + "\""
        " \"" + datPath.string() + "\"";
    return runProcess(cmd, onOutput);
}

bool OniSplitService::copyDat(const std::filesystem::path& datPath,
                              const OutputCallback&        onOutput) const {
    const auto dest = m_oniGamePath / GAME_DATA_FOLDER / DAT_NAME;
    std::error_code ec;
    if (std::filesystem::exists(dest)) {
        std::filesystem::remove(dest, ec);
        if (ec) {
            onOutput("[OniSplit] Failed to remove existing DAT: " + ec.message());
            return false;
        }
    }

    std::filesystem::copy_file(datPath, dest, ec);
    if (ec) {
        onOutput("[OniSplit] Copy failed: " + ec.message());
        return false;
    }
    onOutput("[OniSplit] Copied to: " + dest.string());
    return true;
}

bool OniSplitService::launchOni(const OutputCallback& onOutput) const {
    const auto oniExe = m_oniGamePath / ONI_EXE;
    if (!std::filesystem::exists(oniExe)) {
        onOutput("[OniSplit] Oni.exe not found at: " + oniExe.string());
        return false;
    }

#ifdef _WIN32
#include <windows.h>
    const HINSTANCE result = ShellExecuteW(
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
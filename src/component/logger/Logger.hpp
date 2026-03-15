#pragma once

#include "component/logger/ILogger.hpp"
#include <fstream>
#include <string>

/**
 * @brief Concrete logger that writes to both console and a log file.
 *
 * The log file path is provided at construction time.
 * Each message is prefixed with a timestamp and a severity level.
 */
class Logger final : public ILogger {
public:
    /**
     * @brief Constructs a Logger that writes to the given file path.
     * @param filePath Absolute or relative path to the log file.
     */
    explicit Logger(const std::string& filePath);

    ~Logger() override;

    // Not copyable, not movable: owns a file stream
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    void info(const std::string& message) override;
    void warning(const std::string& message) override;
    void error(const std::string& message) override;
    void separator() override;

private:
    std::ofstream m_fileStream;

    /**
     * @brief Writes a formatted log line to both console and file.
     * @param level Severity label (e.g. "INFO", "WARNING", "ERROR").
     * @param message The message to log.
     */
    void writeLog(const std::string& level, const std::string& message);

    /**
     * @brief Returns the current date and time as a formatted string.
     * @return e.g. "2025-03-15 14:32:01"
     */
    [[nodiscard]] static std::string currentTimestamp();
};

#pragma once

#include <string>

/**
 * @brief Interface for logging services.
 *
 * Depend on this interface instead of the concrete Logger class
 * to allow injection of alternative implementations (e.g. for testing).
 */
class ILogger {
public:
    virtual ~ILogger() = default;

    /**
     * @brief Logs an informational message.
     * @param message The message to log.
     */
    virtual void info(const std::string& message) const = 0;

    /**
     * @brief Logs a warning message.
     * @param message The message to log.
     */
    virtual void warning(const std::string& message) const = 0;

    /**
     * @brief Logs an error message.
     * @param message The message to log.
     */
    virtual void error(const std::string& message) const = 0;

    /**
     * @brief Logs a visual separator in the log.
     */
    virtual void separator() const = 0;
};

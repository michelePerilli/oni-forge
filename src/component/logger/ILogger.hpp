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

    virtual void info(const std::string& message) const = 0;
    virtual void warning(const std::string& message) const = 0;
    virtual void error(const std::string& message) const = 0;
    virtual void separator() const = 0;
};

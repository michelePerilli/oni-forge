#include "component/logger/Logger.hpp"

#include <chrono>
#include <format>
#include <iostream>

Logger::Logger(const std::string& filePath) {
    m_fileStream.open(filePath, std::ios::app);

    if (!m_fileStream.is_open()) {
        std::cerr << "[Logger] Could not open log file: " << filePath << "\n";
    }
}

Logger::~Logger() {
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
}

void Logger::info(const std::string& message) {
    writeLog("INFO", message);
}

void Logger::warning(const std::string& message) {
    writeLog("WARNING", message);
}

void Logger::error(const std::string& message) {
    writeLog("ERROR", message);
}

void Logger::separator() {
    const std::string line = "------------------------------------------------------------";
    std::cout << line << "\n";
    if (m_fileStream.is_open()) {
        m_fileStream << line << "\n";
    }
}

void Logger::writeLog(const std::string& level, const std::string& message) {
    const std::string line = std::format("[{}] [{}] {}", currentTimestamp(), level, message);

    std::cout << line << "\n";

    if (m_fileStream.is_open()) {
        m_fileStream << line << "\n";
    }
}

std::string Logger::currentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    return std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::floor<std::chrono::seconds>(now));
}

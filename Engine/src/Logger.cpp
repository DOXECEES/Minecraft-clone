// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Logger.hpp"

void Logger::Log(const std::string &logMessage, LoggingLevel level)
{
    if (isDebugConsoleEnable)
    {
        LogConsole(logMessage, level);
    }
    LogFile(logMessage, level);
}

std::string Logger::GetLevelS(LoggingLevel level)
{
    switch (level)
    {
    case INFO:
        return "INFO";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    case FATAL:
        return "FATAL";
    default:
        return "UNDEFINED";
    }
}

std::string Logger::GetTime()
{
    std::chrono::time_point<std::chrono::system_clock> tPoint = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(tPoint);
    return std::asctime(std::localtime(&tt));
}

void Logger::LogConsole(const std::string &logMessage, LoggingLevel level)
{
    std::string time = GetTime();
    time.erase(std::remove(time.begin(), time.end(), '\n'), time.end());

    std::cout << "[" << time << "]"
              << "[" << GetLevelS(level) << "] " << logMessage << std::endl;
}

void Logger::LogFile(const std::string &logMessage, LoggingLevel level)
{
    std::string time = GetTime();
    time.erase(std::remove(time.begin(), time.end(), '\n'), time.end());

    file << "[" << time << "]"
         << "[" << GetLevelS(level) << "] " << logMessage << '\n';
}

void Logger::EnableConsoleLogging()
{
    isDebugConsoleEnable = true;
}

void Logger::StartLogging()
{
    file.open("latest.txt");
}

bool Logger::isDebugConsoleEnable = false;
std::ofstream Logger::file;
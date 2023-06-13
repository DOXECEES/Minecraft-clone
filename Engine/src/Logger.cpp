
#include "Logger.hpp"

void Logger::Log(const std::string &logMessage,LoggingLevel level)
{
    if(isDebugConsoleEnable)
    {
        LogConsole(logMessage,level);
    }
    LogFile(logMessage,level);
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


void Logger::LogConsole(const std::string &logMessage,LoggingLevel level)
{
    std::string time = GetTime();
    time.erase(std::remove(time.begin(),time.end(),'\n'),time.end());

    std::cout <<"[" << time << "]"
    << "["<< GetLevelS(level) <<"] " << logMessage;
}

void Logger::LogFile(const std::string &logMessage,LoggingLevel level)
{
    std::ofstream file;
    file.open("latest.txt");

    std::string time = GetTime();
    time.erase(std::remove(time.begin(),time.end(),'\n'),time.end());

    file << "[" << time << "]" << 
        "[" << GetLevelS(level) << "] " << logMessage;
}


void Logger::EnableConsoleLogging()
{
    isDebugConsoleEnable = true;
}


bool Logger::isDebugConsoleEnable = false;

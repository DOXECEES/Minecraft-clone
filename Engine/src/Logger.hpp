
#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

class Logger
{
    public:
        
        enum LoggingLevel
        {
            INFO,
            WARNING,
            ERROR,
            FATAL
        };

        static void Log(const std::string &logMessage,LoggingLevel level);
        static void EnableConsoleLogging();
    
    private:
        static void LogConsole(const std::string &logMessage,LoggingLevel level);
        static void LogFile(const std::string &logMessage,LoggingLevel level);
        static std::string GetLevelS(LoggingLevel level);
        static std::string GetTime();

    private:
        static bool isDebugConsoleEnable; 



};
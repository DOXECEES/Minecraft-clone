
#include <chrono>
#include <fstream>

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

        static void Log();
        static void SetLoggingLevel(LoggingLevel level);

    private:
        static LoggingLevel level;



};
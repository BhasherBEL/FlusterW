//
// Created by Bhasher on 15/05/2020.
//

#ifndef FLUSTERW_LOGGING_H
#define FLUSTERW_LOGGING_H

#include <fstream>
#include <sstream>

class Log {
public:
    Log(std::string const &path);
    Log(std::string const &path, bool console, bool file);
    void logDebug(std::string const &text, bool console, bool file);
    void logDebug(std::string const &text);
    void logDebugC(std::string const &text);
    void logDebugF(std::string const &text);
    void log(std::string const &text, bool console, bool file);
    void log(std::string const &text);
    void logC(std::string const &text);
    void logF(std::string const &text);

    static char const endl{'\n'};
    bool hasConsole{true};
    bool hasFile{true};

private:
    std::ofstream file;
};

template<typename T>
Log& operator<<(Log &log, T const &t){
    std::stringstream sst;
    sst << t;
    log.log(sst.str(), log.hasConsole, log.hasFile);
    return log;
}


#endif //FLUSTERW_LOGGING_H

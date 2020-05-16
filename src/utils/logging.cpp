//
// Created by Bhasher on 15/05/2020.
//

#include "logging.h"

#include <fstream>
#include <iostream>
#include <filesystem>

Log::Log(std::string const &path) {
    Log::hasConsole = true;
    Log::hasFile = true;
    Log::_filename = std::filesystem::path(path).filename().replace_extension().string();
    Log::file = std::ofstream{path};
    if (Log::file.bad() || Log::file.fail()) Log::logDebugC("[ERROR] Log file can't be open.");
}

Log::Log(std::string const &path, bool const console, bool const file){
    Log::hasConsole = console;
    Log::hasFile = file;
    Log::_filename = std::filesystem::path(path).filename().string();
    Log::file = std::ofstream{path};
    if (Log::file.bad() || Log::file.fail()) Log::logDebugC("[ERROR] Log file can't be open.");
}

void Log::logDebug(const std::string &text, bool console=true, bool file=true) {
#ifndef NDEBUG
    Log::log(text, console, file);
#endif
}

void Log::logDebug(const std::string &text) {
#ifndef NDEBUG
    Log::log(text, true, true);
#endif
}

void Log::logDebugC(const std::string &text) {
#ifndef NDEBUG
    Log::log(text, true, false);
#endif
}

void Log::logDebugF(const std::string &text) {
#ifndef NDEBUG
    Log::log(text, false, true);
#endif
}

void Log::log(const std::string &text, bool console=true, bool file=true) {
    if(console) std::cout << text;
    if(file) Log::file << text;
}
void Log::log(const std::string &text) {
    Log::log(text, true, true);
}

void Log::logC(const std::string &text) {
    Log::log(text, true, false);
}

void Log::logF(const std::string &text) {
    Log::log(text, false, true);
}

char Log::endl() {
    Log::_el = true;
    return '\n';
}

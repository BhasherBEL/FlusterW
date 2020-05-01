//
// Created by Bhasher on 26/04/2020.
//

#ifndef CMAKETEST_DB_H
#define CMAKETEST_DB_H


#include <filesystem>

class db {
public:
    static bool copyDB(std::filesystem::path const& source, std::filesystem::path const& dest);
};


#endif //CMAKETEST_DB_H

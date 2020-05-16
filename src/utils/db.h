//
// Created by Bhasher on 26/04/2020.
//

#ifndef FLUSTERW_DB_H
#define FLUSTERW_DB_H

#include <filesystem>

class DB {
public:
    static bool copyDB(std::filesystem::path const& source, std::filesystem::path const& dest);
};


#endif //FLUSTERW_DB_H

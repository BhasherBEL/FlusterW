//
// Created by Bhasher on 26/04/2020.
//

#include <fstream>
#include <filesystem>
#include <exception>
#include <iostream>
#include "db.h"

bool db::copyDB(std::filesystem::path const& source, std::filesystem::path const& dest) {
    try{
        std::ifstream src{source, std::ios::binary};
        std::ofstream dst{dest, std::ios::binary};
        dst << src.rdbuf();
        dst.close();
        src.close();
    } catch (std::exception const& e) {
        std::cerr << "Error copying '" << source.string() << "' database for '" << e.what() << "'" << std::endl;
    }
    return true;
}
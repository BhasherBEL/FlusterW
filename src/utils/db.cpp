//
// Created by Bhasher on 26/04/2020.
//

#include "db.h"

#include <filesystem>

#include "../../contrib/sqlite/sqlite3.h"

/**
 *
 * @param source
 * @param dest
 *
 * @pre source file must exist
 *
 * @return if the operation was successful
 */
bool DB::copyDB(std::filesystem::path const& source, std::filesystem::path const& dest) {
    try{
        std::filesystem::copy(source, dest);
    } catch (std::filesystem::filesystem_error const &error) {
        return false;
    }
    return true;
}



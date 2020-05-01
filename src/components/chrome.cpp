//
// Created by Bhasher on 01/05/2020.
//

#include <iostream>
#include <filesystem>
#include "chrome.h"

/**
 * @brief Execute the *Chrome Component* and extract everything he can from chrome's data.
 * @return if execution finish correctly
 */
bool Chrome::execute() {
    std::cout << "Chrome s'execute !" << std::endl;
    return true;
}

bool Chrome::can() {
    std::filesystem::path chromePath{std::getenv("LOCALAPPDATA")};
    chromePath /= "Google\\Chrome\\User Data\\Default";
    if(!std::filesystem::exists(chromePath)){
        return false;
    }
    Chrome::profilesPath.push_back(chromePath);
    return true;
}

std::vector<Credential> Chrome::getCredentials() {
    return std::vector<Credential>{};
}

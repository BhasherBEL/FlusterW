//
// Created by Bhasher on 01/05/2020.
//

#ifndef FLUSTERW_NAVIGATOR_H
#define FLUSTERW_NAVIGATOR_H

#include <vector>
#include <filesystem>
#include <unordered_map>
#include "component.h"

struct Cookie{

};

struct Credential{

};

class Navigator : public Component {
public:
    using Component::Component;
    //virtual std::unordered_map<std::filesystem::path, std::vector<Cookie>> getCookies() ;
    //virtual std::unordered_map<std::filesystem::path, std::vector<Credential>> getCredentials() ;
protected:
    std::vector<std::filesystem::path> profilesPath{};
};

#endif //FLUSTERW_NAVIGATOR_H

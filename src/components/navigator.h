//
// Created by Bhasher on 01/05/2020.
//

#ifndef FLUSTERW_NAVIGATOR_H
#define FLUSTERW_NAVIGATOR_H

#include <vector>
#include <filesystem>
#include "component.h"

struct Cookie{

};

struct Credential{

};

class Navigator : public Component {
public:
    using Component::Component;
    virtual std::vector<Cookie> getCookies(){return std::vector<Cookie>{};};
    virtual std::vector<Credential> getCredentials(){return std::vector<Credential>{};};
protected:
    std::vector<std::filesystem::path> profilesPath{};
};

#endif //FLUSTERW_NAVIGATOR_H

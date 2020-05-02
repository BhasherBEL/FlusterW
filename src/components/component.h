//
// Created by Bhasher on 01/05/2020.
//

#ifndef FLUSTERW_COMPONENT_H
#define FLUSTERW_COMPONENT_H

#include <string>
#include <vector>
#include <filesystem>
#include "../utils/rand.h"

class Component {
public:
    virtual bool can();
    virtual bool execute();
    std::string const &getName() const {return name;};
    static std::filesystem::path const tmpDir;
    static std::filesystem::path const outputDir;
protected:
    virtual void log(){};
    std::string name;
};


#endif //FLUSTERW_COMPONENT_H

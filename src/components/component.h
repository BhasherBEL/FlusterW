//
// Created by Bhasher on 01/05/2020.
//

#ifndef FLUSTERW_COMPONENT_H
#define FLUSTERW_COMPONENT_H

#include <string>
#include <vector>

class Component {
public:
    Component() = default;
    virtual bool can(){return true;};
    virtual bool execute(){return true;};
    std::string const &getName() const {return name;};
protected:
    virtual void log(){};
    std::string name;
};


#endif //FLUSTERW_COMPONENT_H

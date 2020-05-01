//
// Created by Bhasher on 01/05/2020.
//

#ifndef FLUSTERW_CHROME_H
#define FLUSTERW_CHROME_H

#include "navigator.h"

class Chrome : public Navigator {
public:
    using Navigator::Navigator;
    Chrome::Chrome(){name = "Chrome";}
    bool execute() override;
    bool can() override;
    std::vector<Credential> getCredentials() override;
};


#endif //FLUSTERW_CHROME_H

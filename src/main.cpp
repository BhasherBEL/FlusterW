#include <iostream>
#include "components/chrome.h"

int main(){
    for(ChromiumBasedBrowser const &browser : Chrome::presentBrowsers()){
        std::string const path{browser.getPath()};
        std::cout << "> " << browser.name
                  << "\n    |--> Path: " << path
                  << "\n    |--> Version: " << std::get<0>(Chrome::getVersion(path))
                  << "\n    |--> Profiles:" << std::endl;

        auto [profiles, _] = Chrome::extractLocalState(path);

        for (Profile const &profile : profiles){
            std::cout << "        |--> " << profile.name << std::endl;
            if(!profile.customFullName.empty() && profile.customFullName != "None") std::cout << "            |--> Full Name: " << profile.customFullName << std::endl;
            if(!profile.id.empty() && profile.id != "None") std::cout << "            |--> ID: " << profile.id << std::endl;
            if(!profile.email.empty() && profile.email != "None") std::cout << "            |--> Email: " << profile.email << std::endl;
            if(!profile.pictureUrl.empty() && profile.pictureUrl != "None") std::cout << "            |--> Picture url: " << profile.pictureUrl << std::endl;
            if(profile.lastUsing != -1) std::cout << "            |--> Last using: " << (int)profile.lastUsing << std::endl;
        }
    }
}
#include <iostream>
#include <array>

#include "components/chrome.h"
#include "utils/logging.h"

int main(){
    Log log{"main.txt", false, true};

    for(ChromiumBasedBrowser const &browser : Chrome::presentBrowsers()){
        std::string const path{browser.getPath()};
        log << "> " << browser.name
                  << "\n    |--> Path: " << path
                  << "\n    |--> Version: " << Chrome::getVersion(path)[0]
                  << "\n    |--> Profiles:" << Log::endl;

        auto profiles = Chrome::getProfiles(path);
        for (Profile const &profile : profiles){
            log << "        |--> " << profile.name << Log::endl;
            if(!profile.customFullName.empty() && profile.customFullName != "None")
                log << "            |--> Full Name: " << profile.customFullName << Log::endl;
            if(!profile.id.empty() && profile.id != "None")
                log << "            |--> ID: " << profile.id << Log::endl;
            if(!profile.customShortcutName.empty() && profile.customShortcutName !="None")
                log << "            |--> Pseudo: " << profile.customShortcutName << Log::endl;
            if(!profile.email.empty() && profile.email != "None")
                log << "            |--> Email: " << profile.email << Log::endl;
            if(!profile.pictureUrl.empty() && profile.pictureUrl != "None")
                log << "            |--> Picture url: " << profile.pictureUrl << Log::endl;
            if(profile.lastUsing != -1 && profile.lastUsing != 0)
                log << "            |--> Last using: " << (int)profile.lastUsing << Log::endl;
        }
    }
}
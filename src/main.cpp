#include <iostream>
#include <array>
#include <chrono>
#include <time.h>

#include "components/chrome.h"
#include "utils/logging.h"

int main(){
    srand(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

    Log log{"main.log", true, true};

    for(ChromiumBasedBrowser const &browser : Chrome::presentBrowsers()){
        std::string const path{browser.getPath()};
        log << "> " << browser.name << log.endl();
        log << "\t|--> Path: " << path << log.endl();
        log << "\t|--> Version: " << Chrome::getVersion(path)[0] << log.endl();
        log << "\t|--> Profiles:" << log.endl();

        auto profiles = Chrome::getProfiles(path);
        for (Profile const &profile : profiles){
            log << "\t\t|--> " << profile.name << log.endl();
            if(!profile.customFullName.empty() && profile.customFullName != "None")
                log << "\t\t\t|--> Full Name: " << profile.customFullName << log.endl();
            if(!profile.id.empty() && profile.id != "None")
                log << "\t\t\t|--> ID: " << profile.id << log.endl();
            if(!profile.customShortcutName.empty() && profile.customShortcutName !="None")
                log << "\t\t\t|--> Pseudo: " << profile.customShortcutName << log.endl();
            if(!profile.email.empty() && profile.email != "None")
                log << "\t\t\t|--> Email: " << profile.email << log.endl();
            if(!profile.pictureUrl.empty() && profile.pictureUrl != "None")
                log << "\t\t\t|--> Picture url: " << profile.pictureUrl << log.endl();
            if(profile.lastUsing != -1 && profile.lastUsing != 0)
                log << "\t\t\t|--> Last using: " << (int)profile.lastUsing << log.endl();

            auto const history{Chrome::getHistory(profile.path.string())};
            log << "\t\t\t|--> History: " << history.size() << " entries" << log.endl();
        }
    }
}
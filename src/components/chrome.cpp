//
// Created by Bhasher on 01/05/2020.
//

#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>

#include "chrome.h"

#include "../../contrib/rapidjson/istreamwrapper.h"
#include "../../contrib/rapidjson/document.h"

/**
 * @brief Convert theoretical path to practical path
 * @return the practical path
 */
std::string ChromiumBasedBrowser::getPath() const {
    auto const first{std::string(ChromiumBasedBrowser::location).find('{')+1};
    auto const last{std::string(ChromiumBasedBrowser::location).find('}')};
    std::string const content{std::string(ChromiumBasedBrowser::location).substr(first, last-first)};
    std::string result{ChromiumBasedBrowser::location};
    result.replace(first-1, last-first+2, std::getenv(content.c_str()));
    return result;
}

/**
 * @brief Extract all interesting data from Local State.
 * @details Extract thoses data:
 *  - Profiles data.
 *  - OS encryption key
 *
 * @param Chrome user data path.
 *
 * @pre The chromeUserDataPath path must exist
 * @pre The Local State file must exist
 *
 * @todo copy file
 * @todo scrap plugins
 *
 * @return
 */
std::tuple<std::vector<Profile>, std::string> Chrome::extractLocalState(std::string const &chromeUserDataPath){
    assert(std::filesystem::exists(chromeUserDataPath) && "chromeUserDataPath doesn't exist");
    assert(std::filesystem::exists(chromeUserDataPath + "Local State") && "Local State file doesn't exist");

    std::ifstream ifs(chromeUserDataPath + "Local State");
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document localState;
    localState.ParseStream(isw);

    std::vector<Profile> profiles{};

    if(localState.HasMember("profile") && localState["profile"].HasMember("info_cache")){
        for(auto itr{localState["profile"]["info_cache"].MemberBegin()};itr!=localState["profile"]["info_cache"].MemberEnd();itr++){

            std::string const name{itr->name.GetString()};

            std::filesystem::path const path{std::filesystem::exists(chromeUserDataPath + name) ? chromeUserDataPath + name:""};

            auto valueIfExist = [&itr](std::string const &cName) -> std::string {
                return itr->value.HasMember(cName.c_str()) && itr->value[cName.c_str()].IsString() ? itr->value[cName.c_str()].GetString():"None";
            };

            double const lastUsing{itr->value.HasMember("active_time") && itr->value["active_time"].IsDouble() ? itr->value["active_time"].GetDouble():-1};
            std::string const customName{valueIfExist("gaia_given_name")};
            std::string const customFullName{valueIfExist("gaia_name")};
            std::string const customShortcutName{valueIfExist("shortcut_name")};
            std::string const id{valueIfExist("gaia_id")};
            std::string const pictureName{valueIfExist("gaia_picture_file_name")};
            std::string const pictureUrl{valueIfExist("last_downloaded_gaia_picture_url_with_size")};
            std::string const officialName{valueIfExist("name")};
            std::string const email{valueIfExist("user_name")};

            profiles.push_back({
                                       name,
                                       path,
                                       lastUsing,
                                       customName,
                                       customFullName,
                                       customShortcutName,
                                       id,
                                       pictureName,
                                       pictureUrl,
                                       officialName,
                                       email
                               });

        }
    }

    std::string const encrypted_key{
            localState.HasMember("os_crypt") && localState["os_crypt"].HasMember("encrypted_key") && localState["os_crypt"]["encrypted_key"].IsString() ?
            localState["os_crypt"]["encrypted_key"].GetString() : ""};

    return std::tuple<std::vector<Profile>, std::string>{profiles, encrypted_key};

}

/**
 * @brief Browse the list of potential browsers and return those who are present
 * @return A vector of existing browsers
 */
std::vector<ChromiumBasedBrowser> Chrome::presentBrowsers(){
    std::vector<ChromiumBasedBrowser> presentBrowsers{};
    for(ChromiumBasedBrowser const &chromiumBasedBrowser : chromiumBasedBrowsers){
        if(std::filesystem::exists(chromiumBasedBrowser.getPath())){
            presentBrowsers.push_back(chromiumBasedBrowser);
        }
    }
    return presentBrowsers;
}

std::tuple<int, int, int, int> Chrome::getVersion(const std::string &chromeUserDataPath) {
    assert(std::filesystem::exists(chromeUserDataPath) && "chromeUserDataPath doesn't exist");
    assert(std::filesystem::exists(chromeUserDataPath + "Last Version") && "Last Version file doesn't exist");

    std::ifstream lastVersionFile{chromeUserDataPath + "Last Version"};
    std::tuple<int, int, int, int> version{-1, -1, -1, -1};

    std::string line;
    unsigned short int i=0;
    while(std::getline(lastVersionFile, line, '.')){
        try{
            switch (i) {
                case 0: std::get<0>(version) = std::stoi(line);
                case 1: std::get<1>(version) = std::stoi(line);
                case 2: std::get<2>(version) = std::stoi(line);
                case 3: std::get<3>(version) = std::stoi(line);
            }
        } catch (std::invalid_argument const &e) {
#ifndef NDEBUG
            std::cerr << "A value in 'Last Version' file (located in " << chromeUserDataPath << ") is not a integer." << std::endl;
#endif
        } catch (std::out_of_range const &e) {
#ifndef NDEBUG
            std::cerr << "A value in 'Last Version' file (located in " << chromeUserDataPath << ") is out of range." << std::endl;
#endif
        }
        i++;
    }

    return version;
}

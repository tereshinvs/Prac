#include <iostream>
#include <string>

enum {
    ALPHA1 = '3',
    ALPHA2 = '4',
    BETTA1 = '1',
    BETTA2 = '2'
};

int main() {
    std::string s;
    while (std::cin >> s) {
        unsigned i = 0;
        bool res = true;

        for (i = 0; i < s.size(); i++)
            if (s[i] != ALPHA1 && s[i] != ALPHA2)
                break;
        for (; i < s.size(); i++)
            if (s[i] != BETTA1 && s[i] != BETTA2) {
                std::cout << 0 << std::endl;
                res = false;
                break;
            }

        if (res)
            std::cout << 1 << std::endl;
    }
}
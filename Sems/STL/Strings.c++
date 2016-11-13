#include <string>
#include <iostream>
#include <algorithm>

int main() {
    std::string s;
    while (std::cin >> s) {
        std::string s_orig = s;
        for (auto it = s.end(); it != s.begin(); it--) {
            std::reverse(s.begin(), it);
            if (s == s_orig) {
                s.erase(it, s.end());
                std::cout << s << std::endl;
                break;
            }
            reverse(s.begin(), it);
        }
    }
}
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::string s;
    while (std::cin >> s) {
        unsigned i = 0;
        for (char c: { 'a', '0', 'b', '1' })
            while (s[i] == c)
                i++;
        
        bool res = i == s.size();
        int c_a = std::count(s.begin(), s.end(), 'a');
        int c_b = std::count(s.begin(), s.end(), 'b');
        int c_0 = std::count(s.begin(), s.end(), '0');
        int c_1 = std::count(s.begin(), s.end(), '1');

        res &= c_0 == c_1 && c_0 > 0;
        res &= c_a > 0 && c_b > 0 && c_a == c_b;

        std::cout << (res ? 1 : 0) << std::endl;
    }
    std::cout << std::endl;
}
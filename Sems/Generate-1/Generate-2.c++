#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

std::vector<std::string> res;

void out_str(int lng, std::string s1, std::string s2) {
    if (lng == 0) {
        std::string ts1 = s1, ts2 = s2;
        std::reverse(ts1.begin(), ts1.end());
        std::reverse(ts2.begin(), ts2.end());
        res.push_back(s1 + ts1 + s2 + ts2);
        return;
    }
    out_str(lng - 2, s1, s2 + '1');
    out_str(lng - 2, s1, s2 + '2');
    if (s2.empty()) {
        out_str(lng - 2, s1 + '3', s2);
        out_str(lng - 2, s1 + '4', s2);
    }
}

int main() {
    int k;
    std::cin >> k;
    out_str(k, "", "");

    std::sort(res.begin(), res.end());
    for (std::string &s: res)
        std::cout << s << std::endl;
}
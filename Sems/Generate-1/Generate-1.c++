#include <iostream>
#include <string>

void out_dict(int lng, std::string curs, bool first_part) {
    if (lng == 0) {
        std::cout << curs << std::endl;
        return;
    }
    out_dict(lng - 1, curs + "1", false);
    out_dict(lng - 1, curs + "2", false);
    if (first_part) {
        out_dict(lng - 1, curs + "3", true);
        out_dict(lng - 1, curs + "4", true);
    }
}

int main() {
    int k;
    std::cin >> k;
    out_dict(k, "", true);
}
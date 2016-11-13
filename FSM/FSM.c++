#include <string>
#include <iostream>
#include <set>
#include <algorithm>
#include <map>

int main() {
    std::map< std::pair<std::string, char>, std::string > rules;
    while (true) {
        std::string cur_st;
        std::cin >> cur_st;
        if (cur_st == "END")
            break;
        char c;
        std::string new_st;
        std::cin >> c >> new_st;
        rules[std::make_pair(cur_st, c)] = new_st;
    }

    std::set<std::string> end_states;
    while (true) {
        std::string st;
        std::cin >> st;
        end_states.insert(st);
        if (st == "END")
            break;
    }

    std::string start_st;
    std::cin >> start_st;

    std::string s;
    std::cin >> s;

    std::string cur_st = start_st;
    int step = 0;
    for (char c: s) {
        auto tmp = rules.find(std::make_pair(cur_st, c));
        if (tmp == rules.end()) {
            std::cout << 0 << std::endl << step << std::endl << cur_st << std::endl;
            return 0;
        }
        step++;
        cur_st = tmp->second;
    }
    int res = end_states.find(cur_st) != end_states.end();
    std::cout << res << std::endl << step << std::endl << cur_st << std::endl;
}
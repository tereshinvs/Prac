#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

int main() {
    std::string s;
    std::cin >> s;

    std::string alpha;
    std::cin >> alpha;

    int state_num, start_state;
    std::vector<int> end_states;
    int q;
    std::cin >> state_num >> start_state >> q; start_state--;
    for (int i = 0; i < q; i++) {
        int x;
        std::cin >> x;
        end_states.push_back(x - 1);
    }

    std::map< std::pair<int, char>, int > rules;
    for (int i = 0; i < state_num; i++)
        for (unsigned j = 0; j < alpha.size(); j++) {
            int x;
            std::cin >> x; x--;
            rules[std::make_pair(i, alpha[j])] = x;
        }

    int cur_state = start_state;
    for (char c: s) {
        auto tmp = rules.find(std::make_pair(cur_state, c));
        if (tmp == rules.end()) {
            std::cout << "NO" << std::endl;
            return 0;
        }
        cur_state = tmp->second;
    }

    if (find(end_states.begin(), end_states.end(), cur_state) != end_states.end())
        std::cout << "YES" << std::endl;
    else
        std::cout << "NO" << std::endl;
}
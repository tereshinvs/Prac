#include <iostream>
#include <vector>
#include <cctype>
#include <algorithm>

class Conversion {
    std::string left, right;
    enum { START = 'S' };

    bool is_terminal(const char c) const { return std::islower(c); }
    bool is_non_terminal(const char c) const { return std::isupper(c); }

    template<typename Iterator>
    bool is_regular_compatible(Iterator begin, Iterator end) const {
        return left.size() == 1 && is_non_terminal(left[0]) &&
            std::all_of(begin, --end, [=](char c) { return is_terminal(c); });
    }

    public:
        Conversion(const std::string &_left, const std::string &_right):
        left(_left == "_" ? "" : _left), right(_right == "_" ? "" : _right) {}

        int get_left_length() const { return left.size(); }
        int get_right_length() const { return right.size(); }

        bool left_is_start() const { return left.size() == 1 && left[0] == START; }
        bool right_is_empty() const { return right.size() == 0; }

        bool is_there_start_smb_in_right() const { return right.find(START) != std::string::npos; }

        bool is_right_regular_compatible() const {
            return is_regular_compatible(right.begin(), right.end());
        }

        bool is_left_regular_compatible() const {
            return is_regular_compatible(right.rbegin(), right.rend());
        }

        bool is_context_insensitive_compatible() const {
            return left.size() == 1 && is_non_terminal(left[0]);
        }

        bool is_context_sensitive_compatible() const {
            for (auto it = left.begin(); it != left.end(); it++)
                if (is_non_terminal(*it) && std::equal(left.begin(), it, right.begin()) &&
                    std::equal(it + 1, left.end(), right.end() - (left.end() - it - 1)))
                    return true;
            return false;
        }
};

class Grammar {
    std::vector<Conversion> rule_list;
    mutable std::pair<int, std::string> type;

    bool is_some_type(bool (Conversion::*is_compatible)() const) const {
        return std::all_of(rule_list.begin(), rule_list.end(),
            [=](Conversion it) { return (it.*is_compatible)(); });
    }

    bool is_non_shortened() const {
        bool there_is_start_smb_in_right = std::any_of(rule_list.begin(), rule_list.end(),
            [](Conversion it) { return it.is_there_start_smb_in_right(); });
        bool there_is_start_to_empty = std::any_of(rule_list.begin(), rule_list.end(),
            [](Conversion it) { return it.left_is_start() && it.right_is_empty(); });
        bool res = std::all_of(rule_list.begin(), rule_list.end(),
            [](Conversion it) {
                return it.left_is_start() || it.get_left_length() <= it.get_right_length(); });
        return (!there_is_start_to_empty || !there_is_start_smb_in_right) && res;
    }

    public:
        Grammar(): rule_list(), type(std::make_pair(-1, "")) {}

        void add_rule(const Conversion &rule) {
            rule_list.push_back(rule);
            type = std::make_pair(-1, "");
        }

        const std::pair<int, std::string> &get_grammar_type() const {
            if (type.first == -1) {
                type = std::make_pair(0, "");
                if (is_some_type(&Conversion::is_right_regular_compatible))
                    type = std::make_pair(3, "right-regular");
                else if (is_some_type(&Conversion::is_left_regular_compatible))
                    type = std::make_pair(3, "left-regular");
                else if (is_some_type(&Conversion::is_context_insensitive_compatible))
                    type = std::make_pair(2, "");
                else if (is_non_shortened())
                    type = std::make_pair(1, is_some_type(&Conversion::is_context_sensitive_compatible)
                        ? "context-sensitive" : "non-shortened");
            }
            return type;
        }
};

int main() {
    Grammar grammar;
    std::string left, right;
    while (std::cin >> left >> right)
        grammar.add_rule(Conversion(left, right));

    auto res = grammar.get_grammar_type();
    std::cout << res.first << std::endl << res.second << std::endl;
}
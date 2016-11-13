#include <iostream>
#include <set>
#include <vector>
#include <cctype>
#include <algorithm>

class Symbols {
    public:
        static const std::string EMPTY;
        enum { START = 'S' };
   
        static bool is_terminal(const char c) { return std::islower(c) || std::isdigit(c); }
        static bool is_non_terminal(const char c) { return std::isupper(c); }

        static bool contains_only_these_N(
            const std::string &str, const std::set<char> &char_set) {
            return std::all_of(str.begin(), str.end(),
                [&](char c) { return is_terminal(c) || char_set.find(c) != char_set.end(); });
        }
};
const std::string Symbols::EMPTY = "_";

class Conversion {
    std::string left, right;

    template<typename Iterator>
    bool is_regular_compatible(Iterator begin, Iterator end) const {
        return left.size() == 1 && Symbols::is_non_terminal(left[0]) &&
            std::all_of(begin, --end, [=](char c) { return Symbols::is_terminal(c); });
    }

    public:
        Conversion(const std::string &_left, const std::string &_right):
        left(_left == "_" ? "" : _left), right(_right == "_" ? "" : _right) {}

        const std::string &get_left() const { return left == "" ? Symbols::EMPTY : left; }
        const std::string &get_right() const { return right == "" ? Symbols::EMPTY : right; }
        int get_left_length() const { return left.size(); }
        int get_right_length() const { return right.size(); }

        bool left_is_start() const { return left.size() == 1 && left[0] == Symbols::START; }
        bool left_is_char(const char c) const { return left.size() == 1 && left[0] == c; }
        bool right_is_empty() const { return right.size() == 0; }

        bool contains_only_these_N(const std::set<char> &char_set) const {
            return Symbols::contains_only_these_N(left, char_set) &&
                Symbols::contains_only_these_N(right, char_set);
        }

        bool right_contains_only_these_N(const std::set<char> &char_set) const {
            return Symbols::contains_only_these_N(right, char_set);
        }

        bool is_there_start_smb_in_right() const {
            return right.find(Symbols::START) != std::string::npos;
        }

//*****************************************************************************************************

        bool is_right_regular_compatible() const {
            return is_regular_compatible(right.begin(), right.end());
        }

        bool is_left_regular_compatible() const {
            return is_regular_compatible(right.rbegin(), right.rend());
        }

        bool is_context_insensitive_compatible() const {
            return left.size() == 1 && Symbols::is_non_terminal(left[0]);
        }

        bool is_context_sensitive_compatible() const {
            for (auto it = left.begin(); it != left.end(); ++it)
                if (Symbols::is_non_terminal(*it) && std::equal(left.begin(), it, right.begin()) &&
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

        const Conversion &get_rule(const int rule_number) const { return rule_list[rule_number]; }
        int get_rule_number() const { return rule_list.size(); }

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

        Grammar remove_barren() const {
            std::set<char> non_barren_smb;
            bool added = true;
            while (added) {
                added = false;
                for (char c = 'A'; c <= 'Z'; c++) {
                    if (non_barren_smb.find(c) != non_barren_smb.end())
                        continue;
                    bool non_barrence = std::any_of(rule_list.begin(), rule_list.end(),
                        [&](const Conversion &cnv) {
                            return cnv.left_is_char(c) &&
                                cnv.right_contains_only_these_N(non_barren_smb);
                        });
                    if (non_barrence) {
                        non_barren_smb.insert(c);
                        added = true;
                        break;
                    }
                }
            }

            Grammar res;
            for (const Conversion &cnv: rule_list)
                if (cnv.contains_only_these_N(non_barren_smb))
                    res.add_rule(cnv);
            return res;
        }
};

int main() {
    Grammar grammar;
    std::string left, right;
    while (std::cin >> left >> right)
        grammar.add_rule(Conversion(left, right));

    Grammar res = grammar.remove_barren();
    for (int i = 0; i < res.get_rule_number(); i++)
        std::cout << res.get_rule(i).get_left() << " " << res.get_rule(i).get_right() << std::endl;
}
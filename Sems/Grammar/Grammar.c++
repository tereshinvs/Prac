#include <iostream>
#include <set>
#include <vector>
#include <cctype>
#include <algorithm>
#include <functional>

class Symbols {
    public:
        static const std::string EMPTY;
        enum { START = 'S', START_DASH = '@' };
   
        static bool is_terminal(const char c) { return std::islower(c) || std::isdigit(c); }
        static bool is_non_terminal(const char c) { return std::isupper(c) || c == START_DASH; }

        static bool contains_only_these_N(
            const std::string &str, const std::set<char> &char_set) {
            return std::all_of(str.begin(), str.end(),
                [&](char c) { return is_terminal(c) || char_set.find(c) != char_set.end(); });
        }

        static bool contains_only_this_chars(
            const std::string &str, const std::set<char> &char_set) {
            return std::all_of(str.begin(), str.end(),
                [&](char c) { return char_set.find(c) != char_set.end(); });
        }

        static void replace_all(std::string &str, const char _old, const char _new) {
            std::replace(str.begin(), str.end(), _old, _new);
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

        bool operator<(const Conversion &arg) const {
            if (left < arg.left)
                return true;
            if (left > arg.left)
                return false;
            return right < arg.right;
        }

        bool operator==(const Conversion &arg) const {
            return left == arg.left && right == arg.right;
        }

        const std::string &get_left() const { return left == "" ? Symbols::EMPTY : left; }
        const std::string &get_right() const { return right == "" ? Symbols::EMPTY : right; }
        int get_left_length() const { return left.size(); }
        int get_right_length() const { return right.size(); }

        bool left_is_start() const {
            return left.size() == 1 && (left[0] == Symbols::START || left[0] == Symbols::START_DASH);
        }
        
        bool left_is_char(const char c) const {
            return left.size() == 1 && left[0] == c;
        }
        
        bool left_is_N() const {
            return left.size() == 1 && Symbols::is_non_terminal(left[0]);
        }
        
        bool right_is_empty() const { return right.size() == 0; }
        bool right_contains_char(const char c) const { return right.find(c) < right.size(); }

        bool contains_only_these_N(const std::set<char> &char_set) const {
            return Symbols::contains_only_these_N(left, char_set) &&
                Symbols::contains_only_these_N(right, char_set);
        }

        bool contains_only_this_chars(const std::set<char> &char_set) const {
            return Symbols::contains_only_this_chars(left, char_set) &&
                Symbols::contains_only_this_chars(left, char_set);
        }

        bool right_contains_only_these_N(const std::set<char> &char_set) const {
            return Symbols::contains_only_these_N(right, char_set);
        }

        bool is_there_start_smb_in_right() const {
            return right.find(Symbols::START) != std::string::npos;
        }

        void replace_all(const char _old, const char _new) {
            Symbols::replace_all(left, _old, _new);
            Symbols::replace_all(right, _old, _new);
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

        std::pair< std::vector<std::string>, std::vector<char> > get_ns_transform_vector(
            const std::set<char> &N, const std::set<char> &X) const {
            if (left.size() != 1 || !Symbols::is_non_terminal(left[0]))
                throw "Not available to transform to non-shortened";
            std::pair< std::vector<std::string>, std::vector<char> > res;
            std::string curs = "";
            for (auto it = right.begin(); it != right.end(); it++)
                if (X.find(*it) != X.end()) {
                    res.first.push_back(curs);
                    curs = "";
                    res.second.push_back(*it);
                } else if (Symbols::is_terminal(*it) ||
                    (N.find(*it) != N.end() && X.find(*it) == X.end()))
                    curs += *it;
                else
                    throw "Not available to transform to non-shortened";
            res.first.push_back(curs);
            return res;
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

    std::set<char> filter_symbols(const std::set<char> &char_set,
        std::function<bool(char, std::set<char> &, const Conversion &)> pred,
        const std::set<char> &start_value = std::set<char>()) const {
        std::set<char> res_smb(start_value);
        bool added = true;
        while (added) {
            added = false;
            for (char c: char_set) {
                if (res_smb.find(c) != res_smb.end())
                    continue;
                if (std::any_of(rule_list.begin(), rule_list.end(),
                    [&](const Conversion &cnv) {
                        return pred(c, res_smb, cnv);
                    })) {
                    res_smb.insert(c);
                    added = true;
                    break;
                }
            }
        }
        return res_smb;
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
            if (get_grammar_type().first < 2)
                throw "Not available";
            std::set<char> smb;
            for (char c = 'A'; c <= 'Z'; c++)
                smb.insert(c);
            smb.insert('@');
            smb = filter_symbols(smb,
                [](char c, std::set<char> &res_smb, const Conversion &cnv) {
                    return cnv.left_is_char(c) && cnv.right_contains_only_these_N(res_smb);
                });
            Grammar res;
            for (const Conversion &cnv: rule_list)
                if (cnv.contains_only_these_N(smb))
                    res.add_rule(cnv);
            return res;
        }

        Grammar remove_unreachable() const {
            if (get_grammar_type().first < 2)
                throw "Not available";
            std::set<char> smb;
            for (char c = 'A'; c <= 'Z'; c++)
                smb.insert(c);
            for (char c = 'a'; c <= 'z'; c++)
                smb.insert(c);
            for (char c = '0'; c <= '9'; c++)
                smb.insert(c);
            smb.insert('@');
            std::set<char> tmp; tmp.insert('S'); tmp.insert('@');
            smb = filter_symbols(smb,
                [](char c, std::set<char> &res_smb, const Conversion &cnv) {
                    return cnv.left_is_N() && res_smb.find(cnv.get_left()[0]) != res_smb.end()
                        && cnv.right_contains_char(c);
                }, tmp);
            Grammar res;
            for (const Conversion &cnv: rule_list)
                if (cnv.contains_only_this_chars(smb))
                    res.add_rule(cnv);
            return res;
        }

        Grammar normalize() const {
            return remove_barren().remove_unreachable();
        }

        Grammar to_non_shortened() const {
            if (get_grammar_type().first == 0)
                throw "Not available";

            std::set<char> N;
            for (const Conversion &cnv: rule_list) {
                for (auto c: cnv.get_left())
                    if (Symbols::is_non_terminal(c))
                        N.insert(c);
                for (auto c: cnv.get_right())
                    if (Symbols::is_non_terminal(c))
                        N.insert(c);
            }

            //
            // 1 step
            //
            std::set<char> X;
            for (const Conversion &cnv: rule_list)
                if (cnv.left_is_N() && cnv.right_is_empty())
                    X.insert(cnv.get_left()[0]);

            X = filter_symbols(N,
                [](char c, std::set<char> &res_smb, const Conversion &cnv) {
                    return cnv.left_is_char(c) &&
                        std::all_of(cnv.get_right().begin(), cnv.get_right().end(),
                            [&](char _c) {
                                return res_smb.find(_c) != res_smb.end();
                            });
                }, X);

            //
            // 2 step
            //
            Grammar tmp;
            for (const Conversion &cnv: rule_list)
                if (!cnv.right_is_empty())
                    tmp.add_rule(cnv);
            
            //
            // 3 step
            //
            if (X.find(Symbols::START) != X.end()) {
                tmp.add_rule(Conversion("@", "S"));
                tmp.add_rule(Conversion("@", "_"));
                N.insert('@');
            } else {
                for (Conversion &cnv: tmp.rule_list)
                    cnv.replace_all('S', '@');
                N.erase('S');
                N.insert('@');
            //    X.insert('@');
            }

            //
            // 4 step
            //
            //std::cout << "4 step" << std::endl;
            Grammar res;
            for (const Conversion &cnv: tmp.rule_list)
                try {
                    std::pair< std::vector<std::string>, std::vector<char> > t_v =
                        cnv.get_ns_transform_vector(N, X);
                    if (t_v.second.size() == 0)
                        throw "Empty vector";

                    std::vector<bool> b_v(t_v.second.size(), false);
                    while (true) {
                        std::string stmp = t_v.first[0];
                        for (unsigned i = 0; i < t_v.second.size(); i++) {
                            if (b_v[i])
                                stmp += t_v.second[i];
                            stmp += t_v.first[i + 1];
                        }
                        if (stmp != "")
                            res.add_rule(Conversion(cnv.get_left(), stmp));

                        bool has_next = false;
                        for (auto i = b_v.rbegin(); i != b_v.rend(); i++)
                            if (!*i) {
                                has_next = true;
                                *i = true;
                                for (auto j = b_v.rbegin(); j != i; j++)
                                    *j = false;
                                break;
                            }
                        if (!has_next)
                            break;
                    }
                } catch (const char *) {
                    res.add_rule(cnv);
                }

            //
            // 5 step
            //
            res = res.remove_barren().remove_unreachable();
            std::sort(res.rule_list.begin(), res.rule_list.end());
            res.rule_list.erase(std::unique(res.rule_list.begin(), res.rule_list.end()),
                res.rule_list.end());
            return res;
        }
};

int main() {
    Grammar grammar;
    std::string left, right;
    while (std::cin >> left >> right)
        grammar.add_rule(Conversion(left, right));

    Grammar res = grammar.to_non_shortened();
    for (int i = 0; i < res.get_rule_number(); i++)
        std::cout << res.get_rule(i).get_left() << " " << res.get_rule(i).get_right() << std::endl;
}
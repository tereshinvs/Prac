#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

struct Rule {
	std::string left, right;

	Rule(const std::string &l, const std::string &r):
	left(l), right(r) {}
};

int main() {
	std::vector<Rule> rules;
	std::string l, r;
	while (std::cin >> l >> r)
		rules.push_back(Rule(l, r));

	std::set<char> N, T;
	for (Rule &rule: rules) {
		N.insert(rule.left[0]);
		T.insert(rule.right.back());
		if (rule.right.size() > 1)
			N.insert(rule.right[0]);
	}

	std::map<std::string, int> states;
	std::for_each(N.begin(), N.end(),
	[	&states](char c) { static int k = 0; states[std::string(1, c)] = k++; });
}
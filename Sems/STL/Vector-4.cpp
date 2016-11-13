#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <set>

void process(const std::vector<int> &vect, std::list<int> list) {
	std::vector<int> tmp(vect);
	std::sort(tmp.begin(), tmp.end());

	auto list_it = list.begin();
	auto tmp_it = tmp.begin();
	int orig_list_size = list.size();

	for (int i = 0; tmp_it != tmp.end(); it++, i++) {
	}
}

int main() {
}
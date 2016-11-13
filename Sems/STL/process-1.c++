#include <vector>
#include <list>
#include <iostream>
#include <set>

void process(const std::vector<int> &_v, std::list<int> &_l) {
    std::set<int> s(_v.begin(), _v.end());
    std::list<int> l;

    int i = 1;
    for (auto lit = _l.begin(); lit != _l.end(); lit++, i++)
        if (s.find(i) == s.end())
            l.push_back(*lit);

    std::swap(_l, l);
}
/*
int main() {
    std::list<int> l = { 1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> v = { -1, 0, -1, 0, 0, 0, 4, 5, 17, 7 };
    process(v, l);
    for (auto it = l.begin(); it != l.end(); it++)
        std::cout << *it << " ";
    std::cout << std::endl;
}*/
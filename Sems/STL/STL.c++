#include <vector>
#include <iterator>
#include <iostream>

void process(std::vector<int> &a) {
    auto itl = a.begin(), itr = a.begin(); itr++;
    auto end = a.end();
    if (a.size() % 2 == 0)
        end++;
    for (; itr != end; itl++, itr++, itr++)
        std::swap(*itl, *itr);
    a.erase(itl, a.end());

    for (auto it = a.rbegin(); it != a.rend(); it++)
        std::cout << *it << std::endl;
}
/*
int main() {
    std::vector<int> a = { 0, 1, 2, 3, 4, 5 };
    process(a);
}*/
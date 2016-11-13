#include <vector>
#include <iostream>
#include <iterator>

void process(const std::vector<int> &v1, std::vector<int> &v2) {
    v2.reserve(v2.size() + v1.size());
    auto it = v1.begin();
    auto jt = v2.begin();
    auto end = v2.end();
    for (; it != v1.end() && jt != end; it++, jt++)
        if (*it > *jt)
            v2.push_back(*it);

    for (auto i = v2.begin(); i != v2.end(); i++)
        std::cout << *i << std::endl;

    v2.shrink_to_fit();
}
/*
int main() {
    
}*/
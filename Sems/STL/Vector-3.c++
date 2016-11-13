#include <vector>
#include <iterator>
#include <iostream>

void process(std::vector<int> &v1, const std::vector<int> &v2, int k) {
    for (auto it = v2.begin(); it != v2.end(); it++)
        if (*it > 0 && unsigned(*it) <= v1.size()) {
            auto jt = v1.begin();
            advance(jt, *it - 1);
            *jt *= k;
        }

    std::ostream_iterator<int> out(std::cout, " ");
    copy(v1.begin(), v1.end(), out);
}
/*
int main() {

}*/
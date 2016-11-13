#include <algorithm>
#include <iostream>
#include <vector>

class MyComparator {
    private:
        unsigned get_positive_bit_value(unsigned t) {
            unsigned res = 0;
            for (; t != 0; t >>= 1)
                res += t & 1;
            return res;
        }

    public:
        bool operator()(unsigned a, unsigned b) {
            return get_positive_bit_value(a) < get_positive_bit_value(b);
        }
};

int main() {
    std::vector<unsigned> a;
    unsigned x;
    while(std::cin >> x)
        a.push_back(x);

    std::stable_sort(a.begin(), a.end(), MyComparator());

    for (auto it = a.begin(); it != a.end(); it++)
        std::cout << *it << std::endl;
}
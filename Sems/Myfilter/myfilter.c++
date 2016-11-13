#include <iterator>
#include <vector>
#include <iostream>

template<typename Iterator, typename ValueType = typename Iterator::value_type, typename Predicat>
int myfilter(Iterator begin, Iterator end,
    Predicat predicat, ValueType value = ValueType()) {
    int res = 0;
    for (auto it = begin; it != end; ++it)
        if (predicat(*it)) {
            *it = value;
            res++;
        }
    return res;
}
/*
struct Pred1 {
    bool operator()(int v) const { return v > 10; }
};

int main() {
    std::vector<int> a = { 0, 1, 12, 12, 121, 1, 2,3,4 };
    std::cout << myfilter(a.begin(), a.end(), Pred1(), 5) << std::endl;

    int b[] = { 0, 1, 12, 12, 121, 1, 2,3,4 };
    std::cout << myfilter(b, b + 9, Pred1(), 5) << std::endl;
}*/
#include <iterator>
#include <vector>
#include <iostream>

template<typename U>
typename U::value_type process(const U &arg) {
    auto it = arg.end();
    auto res = typename U::value_type();
    if (it == arg.begin())
        return res;
    else
        it--;
    for (int i = 0; i < 3; i++) {
        res += *it;
        if (it == arg.begin())
            break;
        it--;
    }
    return res;
}
/*
int main() {
    std::vector<int> a  = { 1, 2, 3, 4 };
    std::cout << process(a) << std::endl;
}*/
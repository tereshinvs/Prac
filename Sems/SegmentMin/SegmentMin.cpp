#include <set>
#include <vector>
#include <iostream>

int main() {
    int n, k;
    std::cin >> n >> k;
    std::vector<int> data(n);
    for (int i = 0; i < n; i++)
        std::cin >> data[i];

    std::multiset<int> rnq;
    rnq.insert(data.begin(), data.begin() + k - 1);

    for (int i = k - 1; i < n; i++) {
        rnq.insert(data[i]);
        std::cout << *rnq.begin() << std::endl;
        rnq.erase(rnq.find(data[i - k + 1]));
    }
}
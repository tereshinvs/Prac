#include <algorithm>
#include <iostream>
#include <vector>
#include <iomanip>

class Sum {
    private:
        double sum;
        int num;

    public:
        Sum(): sum(0.0), num(0) {}

        ~Sum() {}

        void operator()(const double x) {
            sum += x;
            num++;
        }

        double get_result() const {
            return sum / num;
        }
};

int main() {
    std::vector<double> data;
    double x;
    while (std::cin >> x)
        data.push_back(x);

    int rem_num = data.size() / 10;
    data.erase(data.begin(), data.begin() + rem_num);
    data.erase(data.end() - rem_num, data.end());

    std::sort(data.begin(), data.end());
    rem_num = data.size() / 10;
    data.erase(data.begin(), data.begin() + rem_num);
    data.erase(data.end() - rem_num, data.end());

    std::cout << std::setprecision(10) << std::for_each(data.begin(), data.end(), Sum()).get_result() << std::endl;
}
#include <iostream>
#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

const double PI = std::acos(-1.0);
const double EPS = 1e-9;

const std::vector< std::function<double(double)> > FUNCS = {
    [](double x) { return std::sin(x); },
    [](double x) { return std::cos(x); },
    [](double x) { return x * x; },
    [](double x) { return std::abs(x); },
    [](double x) { return std::exp(x); },
    [](double x) { return std::sqrt(42.0 + x * x); },
    [](double x) { return x * x * x; },
    [=](double x) { return std::cos(x + PI / 3.0); },
    [=](double x) { return std::sin(x - PI / 42.0); },
    [](double x) { return std::log(2.0 + x * x); }
};

class Function {
    private:
        int func_num;
        std::function<double(double)> calc;

    public:
        Function(const int _func_num = 0):
        func_num(_func_num), calc(FUNCS[func_num]) {}

        ~Function() {}

        double operator()(const double x) const {
            return calc(x);
        }

        int get_num() const {
            return func_num;
        }
};

class Comparator {
    private:
        double y;
        std::function<bool(const Function &, const Function &)> cmp;

    public:
        Comparator(const int _type, const double _y): y(_y), cmp(0) {
            if (_type == 1)
                cmp = [=](const Function &f1, const Function &f2) {
                        double t1 = f1(y), t2 = f2(y);
                        return t1 < t2 || (std::abs(t1 - t2) < EPS && f1.get_num() < f2.get_num());
                    };
            if (_type == 2)
                cmp = [=](const Function &f1, const Function &f2) {
                        double t1 = f1(y), t2 = f2(y);
                        double at1 = std::abs(t1), at2 = std::abs(t2);
                        return at1 < at2 || (std::abs(t1 - t2) < EPS && f1.get_num() > f2.get_num());
                    };
        }

        ~Comparator() {}

        bool operator()(const Function f1, const Function f2) {
            return cmp(f1, f2);
        }
};

int main() {
    std::vector<Function> expr;
    for (int i = 0; i < 10; i++)
        expr.push_back(Function(i));

    double y, z;
    int type;
    std::cin >> type >> y >> z;

    sort(expr.begin(), expr.end(), Comparator(type, y));

    double res = z;
    for (int i = 9; i >= 0; i--)
        res = expr[i](res);

    std::cout << res << std::endl;
}
#include <iostream>
#include <cmath>

class Avg {
    private:
        int n;
        double sum;
        double sqr_sum;

    public:
        Avg(): n(0), sum(0.0), sqr_sum(0.0) {}

        ~Avg() {}

        Avg &operator+=(const double x) {
            n++;
            sum += x;
            sqr_sum += x * x;
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &out, const Avg &avg);
};

std::ostream &operator<<(std::ostream &out, const Avg &avg) {
    double avg1 = avg.sum / avg.n;
    double avg2 = (avg.sqr_sum - 2.0 * avg1 * avg.sum + avg.n * avg1 * avg1) / (avg.n - 1.0);
    out << avg1 << std::endl << avg2 << std::endl;
    return out;
}
/*
int main()
{
    Avg avg;
    double x;
    while ((std::cin >> x)) {
        avg += x;
    }
    std::cout << avg;
    return 0;
}
*/
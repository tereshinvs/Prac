#include <string>
#include <cmath>
#include <sstream>
#include <iostream>

class Rational {
    private:
        int a, b;

        int gcd(int ta, int tb) {
            return tb == 0 ? ta : gcd(tb, ta % tb);
        }

        void normalize() {
            int tgcd = gcd(std::abs(a), std::abs(b));
            a /= tgcd;
            b /= tgcd;

            if (b < 0) {
                a = -a;
                b = -b;
            }
        }

    public:
        Rational(const int _a = 0, const int _b = 1): a(_a), b(_b) {
            normalize();
        }

        ~Rational() {}

        Rational &Add(const Rational &t) {
            a = a * t.b + t.a * b;
            b = b * t.b;
            normalize();
            return *this;
        }

        Rational &Substract(const Rational &t) {
            a = a * t.b - t.a * b;
            b = b * t.b;
            normalize();
            return *this;
        }

        Rational &Multiply(const Rational &t) {
            a *= t.a;
            b *= t.b;
            normalize();
            return *this;
        }

        Rational &Divide(const Rational &t) {
            a *= t.b;
            b *= t.a;
            normalize();
            return *this;
        }

        bool EqualTo(const Rational &t) const {
            return a == t.a && b == t.b;
        }

        int CompareTo(const Rational &t) const {
            int tmp = a * t.b - b * t.a;
            if (tmp < 0)
                return -1;
            if (tmp > 0)
                return 1;
            return 0;
        }

        bool IsInteger() const {
            return a % b == 0;
        }

        std::string ToString() const {
            std::stringstream stream;
            stream << a;
            if (b != 1)
                stream << "/" << b;
            std::string res;
            stream >> res;
            return res;
        }
};

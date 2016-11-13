//#include <iostream>

template<int N>
class Fib {
    public:
        enum {
            VAL = Fib<N - 1>::VAL + Fib<N - 2>::VAL
        };
};

template<>
class Fib<1> {
    public:
        enum {
            VAL = 1
        };
};

template<>
class Fib<0> {
    public:
        enum {
            VAL = 0
        };
};
/*
int main() {
    std::cout << Fib<5>::VAL << std::endl;
}*/
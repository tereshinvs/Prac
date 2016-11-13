#include <iostream>
#include <string>
#include <type_traits>

template<typename T>
class IsClass {
    private:
        struct char2 {
            char int2[2];
        };

        template<typename U>
        static char check(void(U::*)());

        template<typename U>
        static char2 check(...);

    public:
        enum {
            value = sizeof(check<T>(0)) == sizeof(char)
        };
};
/*
int main() {
    std::cout << IsClass<int>::value << std::endl; // false
    std::cout << IsClass<std::string>::value << std::endl; // true
}*/
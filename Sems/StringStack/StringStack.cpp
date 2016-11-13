#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class StringStack {
    private:
        std::vector<std::string> data;

        void swap(StringStack &tmp) {
            std::swap(data, tmp.data);
        }

    public:
        StringStack() {}
        StringStack(const std::string &t) {
            data.push_back(t);
        }

        ~StringStack() {}

        unsigned size() const {
            return data.size();
        }

        std::string pop() {
            std::string res = data.back();
            data.resize(data.size() - 1);
            return res;
        }

        StringStack &push(const std::string &tmp) {
            data.push_back(tmp);
            return *this;
        }

        StringStack &operator=(const StringStack &arg) {
            StringStack tmp(arg);
            swap(tmp);
            return *this;
        }

        StringStack &operator=(const std::string &arg) {
            return *this = StringStack(arg);
        }

        StringStack &operator+=(const StringStack &arg) {
            for (unsigned i = 0; i < arg.size(); i++)
                push(arg.data[i]);
            return *this;
        }

        StringStack &operator+=(const std::string &arg) {
            return *this += StringStack(arg);
        }

        StringStack &operator,(const StringStack &arg) {
            return *this += arg;
        }

        StringStack &operator,(const std::string &arg) {
            return *this += StringStack(arg);
        }

        friend std::ostream &operator<<(std::ostream &, const StringStack &);
        friend StringStack &operator,(StringStack &, StringStack &);
};

std::ostream &operator<<(std::ostream &out, const StringStack &arg) {
    for (int i = arg.size() - 1; i >= 0; i--)
        out << arg.data[i] << std::endl;
    return out;
}
/*
int main() {
    StringStack s;
    s.push("abc");

    std::string str;
    s.push(str);

    str = s.pop();

    s = "123"; // <=> очистить стек и поместитть в него строку 123

    s = "abc", "cde", "fde"; // в стеке на дне должно быть "abc", ..., на вершине "fde"

    s = "str1";
    s += "str2", "str3";
    std::cout << s;
}*/
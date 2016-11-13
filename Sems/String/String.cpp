#include <string>
#include <set>
#include <iostream>
#include <iterator>

enum {
    DATA_IS_HERE = true,
    DATA_SOMEWHERE_ELSE = false
};

class String {
    private:
        bool type;
        mutable std::string data;
        String *father;
        mutable std::set<String *> refs;

        void delete_this_vertex() {
            if (type == DATA_IS_HERE) {
                if (!refs.empty()) {
                    (*refs.begin())->data = data;
                    (*refs.begin())->type = DATA_IS_HERE;
                    (*refs.begin())->father = *refs.begin();

                    for (auto it = ++refs.begin(); it != refs.end(); it++) {
                        (*it)->father = *refs.begin();
                        (*refs.begin())->refs.insert(*it);
                    }
                }
            } else
                father->refs.erase(this);
            father = 0;
            refs.clear();
        }

        std::string &get_string_ref() const {
            return father->data;
        }

    public:
        String(const char *arg = ""):
        type(DATA_IS_HERE), data(arg), father(this), refs() {}

        String(const String &arg):
        type(DATA_SOMEWHERE_ELSE), data(""), father(arg.father), refs() {
            father->refs.insert(this);
        }

        ~String() {
            delete_this_vertex();
        }

        operator std::string() const {
            return get_string_ref();
        }

        const String &operator+=(const std::string &arg) {
            std::string tmp = get_string_ref();
            delete_this_vertex();
            type = DATA_IS_HERE;
            data = tmp + arg;
            father = this;
            return *this;
        }

        const char &operator[](const unsigned i) const {
            return get_string_ref()[i];
        }

        char &operator[](const unsigned i) {
            data = get_string_ref();
            delete_this_vertex();
            type = DATA_IS_HERE;
            father = this;
            return data[i];
        }

        const String &operator=(const String &arg) {
            if (get_string_ref() == arg.get_string_ref())
                return *this;
            delete_this_vertex();
            type = DATA_SOMEWHERE_ELSE;
            data = "";
            father = arg.father;
            father->refs.insert(this);
            return *this;
        }
};                  
  
int main() {
    String a = "abc", b = a, c = b;
    String x = "xyz", y = x, z = y;

    b = c;
    a = c;
    c = c;

    x = b;
    a = z;

    z = x;
    a = z;

    std::cout << std::string(a) << std::endl;
    std::cout << std::string(b) << std::endl;
    std::cout << std::string(c) << std::endl;

    std::cout << std::string(x) << std::endl;
    std::cout << std::string(y) << std::endl;
    std::cout << std::string(z) << std::endl;
}
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <vector>

class Formatter {
    public:
        virtual ~Formatter() {}

        virtual std::string format(const std::tm &) {
            return "";
        }
};

class Formatter1: public Formatter {
    public:
        virtual std::string format(const std::tm &t) {
            std::stringstream stream;
            stream << std::setfill('0');
            stream << std::setw(2) << t.tm_mday << "."
                << std::setw(2) << t.tm_mon + 1<< "."
                << std::setw(4) << t.tm_year + 1900;
            return stream.str();
        }
};

class Formatter2: public Formatter {
    public:
        virtual std::string format(const std::tm &t) {
            std::stringstream stream;
            stream << std::setfill('0');
            stream << std::setw(4) << t.tm_year + 1900 << "-"
            << std::setw(2)    << t.tm_mon + 1 << "-"
            << std::setw(2) << t.tm_mday;
            return stream.str();
        }
};

class Formatter3: public Formatter {
    public:
        virtual std::string format(const std::tm &t) {
            const std::vector<std::string> month_names = {
                "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
            };
            std::stringstream stream;
            stream << t.tm_mday << " " << month_names[t.tm_mon] << " " << t.tm_year + 1900;
            return stream.str();
    }
};

Formatter *get_formatter(const int type) {
    switch (type) {
        case 1: return new Formatter1();
        case 2: return new Formatter2();
        case 3: return new Formatter3();
        default: return new Formatter();
    }
}

class DateFormatter {
    private:
        int type;
        Formatter *formatter;

    public:
        DateFormatter(const int _type):
        type(_type), formatter(get_formatter(_type)) {}

        DateFormatter(const DateFormatter &arg):
        type(arg.type), formatter(get_formatter(arg.type)) {}

        ~DateFormatter() {
            delete formatter;
        }

        std::string printDate(const std::tm &t) const {
            return formatter->format(t);
        }

        DateFormatter &operator=(const DateFormatter &arg) {
            type = arg.type;
            if (formatter)
                delete formatter;
            formatter = get_formatter(type);
            return *this;
        }
};
/*
void f(DateFormatter *df)
{
    std::cout << "some code 1" << std::endl;

    std::time_t time = std::time(NULL);
    std::tm t= *std::localtime(&time);

    std::cout << df->printDate(t) << std::endl;

    std::cout << "some code 2" << std::endl;
}
*/
void run(const char *path) {
    std::ifstream fin;
    fin.open(path);
    int tmp;
    fin >> tmp;
    fin.close();
    DateFormatter *formater = new DateFormatter(tmp);
    f(formater);
    delete formater;
}
/*
int main() {
    run("input.txt");
}                                  */
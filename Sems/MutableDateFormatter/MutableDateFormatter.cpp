#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <vector>

class DateFormatter {
    public:
        virtual ~DateFormatter() {}

        virtual std::string printDate(const std::tm &) {
            return "";
        }
};

class DateFormatter1: public DateFormatter {
    public:
        virtual std::string printDate(const std::tm &t) {
            std::stringstream stream;
            stream << std::setfill('0');
            stream << std::setw(2) << t.tm_mday << "."
                << std::setw(2) << t.tm_mon + 1<< "."
                << std::setw(4) << t.tm_year + 1900;
            return stream.str();
        }
};

class DateFormatter2: public DateFormatter {
    public:
        virtual std::string printDate(const std::tm &t) {
            std::stringstream stream;
            stream << std::setfill('0');
            stream << std::setw(4) << t.tm_year + 1900 << "-"
            << std::setw(2)    << t.tm_mon + 1 << "-"
            << std::setw(2) << t.tm_mday;
            return stream.str();
        }
};

class DateFormatter3: public DateFormatter {
    public:
        virtual std::string printDate(const std::tm &t) {
            const std::vector<std::string> month_names = {
                "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
            };
            std::stringstream stream;
            stream << t.tm_mday << " " << month_names[t.tm_mon] << " " << t.tm_year + 1900;
            return stream.str();
    }
};

DateFormatter *get_formatter(const int type) {
    switch (type) {
        case 1: return new DateFormatter1();
        case 2: return new DateFormatter2();
        case 3: return new DateFormatter3();
        default: return new DateFormatter();
    }
}

class MutableDateFormatter: public DateFormatter {
    private:
        DateFormatter *formatter;

        void delete_formatter() {
            if (formatter != 0)
                delete formatter;
        }

    public:
        MutableDateFormatter(const int type = 0):
        formatter(get_formatter(type)) {}

        MutableDateFormatter(const MutableDateFormatter &arg):
        formatter(new DateFormatter(*arg.formatter)) {}

        ~MutableDateFormatter() {
            delete_formatter();
        }

        virtual std::string printDate(const std::tm &t) {
            return formatter->printDate(t);
        }

        void toType(const int new_type) {
            delete_formatter();
            formatter = get_formatter(new_type);
        }

        MutableDateFormatter &operator=(const MutableDateFormatter &arg) {
            delete_formatter();
            formatter = new DateFormatter(*arg.formatter);
            return *this;
        }
};
/*
void g(DateFormatter *df, const std::tm &t)
{
    std::string s = df->printDate(t);
}

void f(MutableDateFormatter *df)
{
    std::cout << "some code 1" << std::endl;

    std::time_t time = std::time(NULL);
    std::tm t= *std::localtime(&time);

    std::cout << df->printDate(t) << std::endl;
    df->toType(2);
    g(df, t);
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
    MutableDateFormatter *formater = new MutableDateFormatter(tmp);
    f(formater);
    delete formater;
}
/*
int main() {
    run("input.txt");
}                                  */
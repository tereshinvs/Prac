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

class DateFormatter1: public Formatter {
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

class DateFormatter2: public Formatter {
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

class DateFormatter3: public Formatter {
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

class TimeFormatter1: public Formatter {
    public:
        virtual std::string format(const std::tm &t) {
            std::stringstream stream;
            stream << t.tm_hour << ":" << t.tm_min;
            return stream.str();
        }
};

class TimeFormatter2: public Formatter {
    public:
        virtual std::string format(const std::tm &t) {
            std::stringstream stream;
            stream << std::setfill('0');
            if (t.tm_hour < 12)
                stream << std::setw(2) << t.tm_hour << ":" << std::setw(2) << t.tm_min << " AM";
            else
                stream << std::setw(2) << t.tm_hour - 12 << ":" << std::setw(2) << t.tm_min << " PM";
            return stream.str();
        }
};

Formatter *get_date_formatter(const int type) {
    switch (type) {
        case 1: return new DateFormatter1();
        case 2: return new DateFormatter2();
        case 3: return new DateFormatter3();
        default: return new Formatter();
    }
}

Formatter *get_time_formatter(const int type) {
    switch (type) {
        case 1: return new TimeFormatter1();
        case 2: return new TimeFormatter2();
        default: return new Formatter();
    }
}

class DateFormatter {
    private:
        int type;
        Formatter *formatter;

    public:
        DateFormatter(const int _type):
        type(_type), formatter(get_date_formatter(_type)) {}

        DateFormatter(const DateFormatter &arg):
        type(arg.type), formatter(get_date_formatter(arg.type)) {}

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
            formatter = get_date_formatter(type);
            return *this;
        }
};

class TimeFormatter {
    private:
        int type;
        Formatter *formatter;

    public:
        TimeFormatter(const int _type):
        type(_type), formatter(get_time_formatter(_type)) {}

        TimeFormatter(const TimeFormatter &arg):
        type(arg.type), formatter(get_time_formatter(arg.type)) {}

        ~TimeFormatter() {
            delete formatter;
        }

        std::string printTime(const std::tm &t) const {
            return formatter->format(t);
        }

        TimeFormatter &operator=(const TimeFormatter &arg) {
            type = arg.type;
            if (formatter)
                delete formatter;
            formatter = get_time_formatter(type);
            return *this;
        }
};
/*
void f(DateFormatter *df, TimeFormatter *tf)
{
    std::cout << "some code 1" << std::endl;

    std::time_t time = std::time(NULL);
    std::tm t= *std::localtime(&time);

    std::cout << df->printDate(t) << tf->printTime(t)  <<  std::endl;

    std::cout << "some code 2" << std::endl;
}
*/
void run(const char *path) {
    std::ifstream fin;
    fin.open(path);
    int tmp;
    fin >> tmp;
    fin.close();
    DateFormatter *date_formatter = new DateFormatter(tmp);
    TimeFormatter *time_formatter = new TimeFormatter(tmp);
    f(date_formatter, time_formatter);
    delete date_formatter;
    delete time_formatter;
}
/*
int main() {
    run("input.txt");
}*/
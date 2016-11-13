#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

class DateFormater {
    private:
        int format_type;

    public:
        DateFormater(const int _format_type = 1):
        format_type(_format_type) {}

        ~DateFormater() {}

        std::string printDate(const std::tm &t) const {
            std::stringstream stream;
            stream << std::setfill('0');
            if (format_type == 1)
                stream << std::setw(2) << t.tm_mday << "."
                    << std::setw(2) << t.tm_mon + 1<< "."
                    << std::setw(4) << t.tm_year + 1900;
            else
                stream << std::setw(4) << t.tm_year + 1900 << "-"
                    << std::setw(2)    << t.tm_mon + 1 << "-"
                    << std::setw(2) << t.tm_mday;
            return stream.str();
        }
};
/*
void f(DateFormater *df)
{
    std::cout << "some code 1" << std::endl;

    std::time_t time = std::time(NULL);
    std::tm t= *std::localtime(&time);

    std::cout << df->printDate(t) << std::endl;

    std::cout << "some code 2" << std::endl;
} */

void run(const char *path) {
    std::ifstream fin;
    fin.open(path);
    int tmp;
    fin >> tmp;
    fin.close();
    DateFormater *formater = new DateFormater(tmp);
    f(formater);
    delete formater;
}
/*
int main() {
    run("input.txt");
} */
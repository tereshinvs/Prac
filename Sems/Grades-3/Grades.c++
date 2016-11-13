#include <set>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream>
#include <map>

const unsigned OUTPUT_WIDTH = 20;

class Date {
    private:
        int year, month, day;

    public:
        Date(const std::string &s): year(0), month(0), day(0) {
            char c;
            std::stringstream tmp;
            tmp << s;
            tmp >> year >> c >> month >> c >> day;
        }

        operator std::string() const {
            std::stringstream tmp;
            tmp << std::setfill('0') << std::setw(4) << year << "/" <<
                std::setw(2) << month << "/" <<
                std::setw(2) << day;
            return tmp.str();
        }

        bool operator<(const Date &d) const {
            if (year < d.year)
                return true;
            if (year > d.year)
                return false;
            if (month < d.month)
                return true;
            if (month > d.month)
                return false;
            return day < d.day;
        }
};

int main() {
    std::ifstream in;
    in.open("02-3.in");

    std::map< std::string, std::map< Date, int> > marks;
    std::set<Date> date_set;

    std::string fname = "", tdatestr = "";
    int tmark = -1;
    while (in >> fname >> tdatestr >> tmark) {
        Date tmp(tdatestr);
        date_set.insert(tmp);
        marks[fname][tmp] = tmark;
    }
    in.close();

    std::ofstream out;
    out.open("02-3.out");

    out << std::setw(OUTPUT_WIDTH) << ".";
    for (const Date &date: date_set)
        out << std::setw(OUTPUT_WIDTH) << std::string(date);
    out << std::endl;

    for (const std::pair< std::string, std::map<Date, int> > &pupil: marks) {
        out << std::setw(OUTPUT_WIDTH) << pupil.first;
        for (const Date &date: date_set) {
            std::map<Date, int>::const_iterator it = pupil.second.find(date);
            if (it != pupil.second.cend())
                out << std::setw(OUTPUT_WIDTH) << it->second;
            else
                out << std::setw(OUTPUT_WIDTH) << ".";
        }
        out << std::endl;
    }
    out.close();
}
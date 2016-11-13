#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>

class MarkList {
    private:
        std::vector<int> marks;
        int sum;
        std::string file_name;
        std::string name;

    public:
        MarkList(const std::string &_file_name, const std::string &_name):
        marks(), sum(0), file_name(_file_name), name(_name) {}

        ~MarkList() {}

        void add_mark(const int mark) {
            marks.push_back(mark);
            sum += mark;
        }

        double get_average() const {
            return sum / double(marks.size());
        }

        const std::vector<int> &get_marks() const {
            return marks;
        }

        const std::string &get_file_name() const {
            return file_name;
        }

        const std::string &get_name() const {
            return name;
        }
};

int main(int argc, char *argv[]) {
    std::vector<MarkList> data;
    for (int i = 1; i < argc; i++) {
        std::ifstream in;
        in.open(argv[i]);
        std::string name;
        while (in >> name) {
            data.push_back(MarkList(argv[i], name));
            int mark;
            while (true) {
                in >> mark;
                if (mark == 0)
                    break;
                data.back().add_mark(mark);
            }
        }
        in.close();
    }

    double average_sum = 0.0;
    for (auto it: data)
        average_sum += it.get_average();
    double average = average_sum / double(data.size());

    std::ofstream out;
    out.open("02-1.out");
    for (auto it: data)
        if (it.get_average() >= average) {
            out << it.get_file_name() << " " << it.get_name();
            const std::vector<int> &marks = it.get_marks();
            for (auto jt = marks.begin(); jt != marks.end(); jt++)
                out << " " << *jt;
            out << std::endl;
        }
    out.close();
}
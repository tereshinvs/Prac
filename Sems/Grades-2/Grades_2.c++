#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>

int main() {
    std::map<std::string, std::pair<int, int> > data;

    std::ifstream in;
    in.open("02-2.in");

    std::string name;
    int mark;
    while (in >> name >> mark) {
        data[name].first += mark;
        data[name].second++;
    }
    in.close();

    std::ofstream out;
    out.open("02-2.out");
    for (auto it = data.begin(); it != data.end(); it++)
        out << it->first << " " << it->second.first / (it->second.second + 0.0) << std::endl;
    out.close();
}
#include <algorithm>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

int main(int argc, char *argv[]) {
	std::map<std::string, int> data;
	for (int i = 0; i < argc; i++) {
		ifstream in;
		in.open(argv[i]);

		std::string word;
		while (in >> word)
			data[word]++;

		in.close();
	}

	std::cout << data.size() << std::endl;

	std::vector< std::pair<int, std::string> > res;
	for (auto it: data)
		res.push_back(std::make_pair(it->second, it->first));
	std::sort(res.begin(), res.end());

	int counter = 20;
	for (auto it = res.crbegin(); it != res.crend() && counter; it++, counter--)
		std::cout << it->second << " " << it->first << std::endl;
}
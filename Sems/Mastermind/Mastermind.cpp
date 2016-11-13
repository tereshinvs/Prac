#include <vector>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <string>
#include <ctime>
#include <numeric>

enum {
	PLACES = 4,
	COLOURS = 6,
	START_NUMBER = '0',
	LAST_NUMBER = '5'
};

class Mastermind {
	private:
		std::vector<char> target;
		int moves, places;
		bool won;

		int get_black_hit(const std::string &turn_str) const {
			return std::inner_product(target.begin(), target.end(), turn_str.begin(), 0,
				std::plus<int>(),
				[](char c1, char c2) { return int(c1 == c2); });
		}

		int get_white_hit(std::string turn_str) const {
			return std::inner_product(target.begin(), target.end(), turn_str.begin(), 0,
				std::plus<int>(),
				[&turn_str](char c1, char c2) {
					if (c1 == c2)
						return 0;
					auto i = turn_str.find(c1);
					if (i == std::string::npos)
						return 0;
					turn_str[i] = '#';
					return 1;
				});
		}

		std::pair<int, int> check_turn(const std::string &turn_str) {
			int black_hit = get_black_hit(turn_str);
			int white_hit = get_white_hit(turn_str);
			if (black_hit == places)
				won = true;
			return std::make_pair(black_hit, white_hit);
		}

	public:
		Mastermind(const int _places, const int colours, const int seed = 123456):
			moves(0), places(_places), won(false), target(_places) {
			std::srand(seed);
			std::generate(target.begin(), target.end(),
				[colours]() {
					int rnd_res = std::rand() % colours;
					if (rnd_res < 10)
						return rnd_res + '0';
					else if (rnd_res < 36)
						return rnd_res - 10 + 'A';
					else
						return rnd_res - 36 + 'a';
				} );
		}

		void out() const {
			for (auto c: target)
				std::cout << c;
			std::cout << std::endl;
		}

		~Mastermind() {}

		std::pair<int, int> turn(const std::string &turn_str, bool need_out = false) {
			moves++;
			return check_turn(turn_str);
		}

		bool is_won() const {
			return won;
		}

		int get_moves() const {
			return moves;
		}
};

int main() {
	Mastermind game = Mastermind(PLACES, COLOURS, time(0));
	game.out();
	while (1) {
		std::cout << "Turn #" << game.get_moves() + 1 << ": "
			"please input a " << PLACES <<
			"-symbol lenght string containing symbols from " <<
			char(START_NUMBER) << " to " << char(LAST_NUMBER) <<
			"." << std::endl;

		std::string turn_str;
		std::cin >> turn_str;
		std::pair<int, int> res = game.turn(turn_str);

		if (game.is_won()) {
			std::cout << "Won in " << game.get_moves() << " turns!" << std::endl;
			break;
		} else
			std::cout << "Black hit: " << res.first <<
				", white hit: " << res.second << ".";
		std::cout << std::endl;
	}
}
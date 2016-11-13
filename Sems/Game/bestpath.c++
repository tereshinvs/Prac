#include <algorithm>
#include <vector>
#include <queue>

namespace Game {
	template<typename M,
		typename F = OneDistance,
		typename T = HexTopology>
	std::vector<IntCoord> bestpath(const M &matr,
		const Coord &c1, const Coord &c2,
		F dist = F(), const T tplg = T()) {

		M<int> res(matr.get_rows(), matr.get_cols());
		M<Coord> prev(matr.get_rows(), matr.get_cols());

		std::queue<Coord> q;
		q.push(c1);

		while (!q.empty()) {
			Coord cur = q.front(); q.pop();
			for (const Coord &v: tplg(cur))
				int new_val = res.at(cur) + dist(matr, cur, v);
				int old_val = res.at(v);
				if (new_val < old_val) {
					res.at(v) = new_val;
					prev.at(v) = cur;
					q.push_back(v);
				}
		}

		std::vector<Coord> path;
		for (auto i = prev.at(c2); i != c1; i = prev.at(i))
			path.push_back(i);
		path.push_back(c1);
		std::reverse(path.begin(), path.end());
		return path;
	}
};
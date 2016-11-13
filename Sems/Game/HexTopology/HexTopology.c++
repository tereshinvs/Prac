#include <stdexcept>
#include <vector>
#include <new>
#include <memory>
#include <algorithm>

template <typename T>
class HexTopology {
    private:
        int row_count, col_count;

        bool is_correct_coord(const std::pair<int, int> &t) const {
            return t.first >= 0 && t.second >= 0 &&
                t.first < row_count && t.second < col_count;
        }

    public:
        HexTopology(const T &m):
            HexTopology(m.get_rows(), m.get_cols()) {}

        HexTopology(int row_count_, int col_count_):
            row_count(row_count_), col_count(col_count_) {}

        std::vector< std::pair<int, int> > operator() (int row, int col) const {
            if (!is_correct_coord(std::make_pair(row, col)))
                throw std::range_error("Range error");
            std::vector<std::pair<int, int> > tmp;
            if (col % 2 == 0)
                tmp = { std::make_pair(-1, 0), std::make_pair(0, 1),
                    std::make_pair(1, 1), std::make_pair(1, 0),
                    std::make_pair(1, -1), std::make_pair(0, -1)
                };
            else
                tmp = { std::make_pair(-1, 0), std::make_pair(-1, 1),
                    std::make_pair(0, 1), std::make_pair(1, 0),
                    std::make_pair(0, -1), std::make_pair(-1, -1)
                };
            std::vector< std::pair<int, int> > res;
            for (std::pair<int, int> &coord: tmp) {
                auto n_cell = std::make_pair(row + coord.first, col + coord.second);
                if (is_correct_coord(n_cell))
                    res.push_back(n_cell);
            }
            std::sort(res.begin(), res.end());
            return res;
        }
};

//int main() {}
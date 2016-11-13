#include <iostream>

namespace Game {
    template<typename T>
    class Coord {
        private:
            T row;
            T col;

        public:
            typedef T value_type;

            Coord<T>(const T &_row = T(), const T &_col = T()):
                row(_row), col(_col) {}

            T get_row() const {
                return row;
            }

            T get_col() const {
                return col;
            }

            bool operator==(const Coord<T> &t) const {
                return row == t.row && col == t.col;
            }

            bool operator!=(const Coord<T> &t) const {
                return row != t.row || col != t.col;
            }

            bool operator<(const Coord<T> &t) const {
                return row < t.row || (row == t.row && col < t.col);
            }

            bool operator<=(const Coord<T> &t) const {
                return row < t.row || (row == t.row && col <= t.col);
            }

            bool operator>(const Coord<T> &t) const {
                return row > t.row || (row == t.row && col > t.col);
            }

            bool operator>=(const Coord<T> &t) const {
                return row > t.row || (row == t.row && col >= t.col);
            }
    };

    template<typename T>
    std::ostream &operator<<(std::ostream &out, const Coord<T> &t) {
        out << "(" << t.get_row() << "," << t.get_col() << ")";
        return out;
    }

    typedef Coord<int> IntCoord;

    //****************************************************************
    // HexTopology
    //****************************************************************

    template <typename T>
    class HexTopology {
        private:
            int row_count, col_count;

            bool is_correct_coord(const IntCoord &t) const {
                return t.first >= 0 && t.second >= 0 &&
                    t.first < row_count && t.second < col_count;
            }

        public:
            HexTopology(const T &m):
                HexTopology(m.get_rows(), m.get_cols()) {}

            HexTopology(int row_count_, int col_count_):
                row_count(row_count_), col_count(col_count_) {}

            typedef T value_type;

            std::vector<IntCoord> operator()(IntCoord coord) const {
                if (!is_correct_coord(coord)
                    throw std::range_error("Range error");

                std::vector<IntCoord> tmp;
                if (col % 2 == 0)
                    tmp = { IntCoord(-1, 0), IntCoord(0, 1),
                        IntCoord(1, 1), IntCoord(1, 0),
                        IntCoord(1, -1), IntCoord(0, -1)
                    };
                else
                    tmp = { IntCoord(-1, 0), IntCoord(-1, 1),
                        IntCoord(0, 1), IntCoord(1, 0),
                        IntCoord(0, -1), IntCoord(-1, -1)
                    };

                std::vector<IntCoord> res;
                std::for_each(tmp.begin(), tmp.end(),
                    [&](IntCoord &t) {
                        auto temp = Coord(coord.get_row() + t.get_row(),
                            coord.get_col() + t.get_col());
                        if (is_correct_coord(temp))
                            res.push_back(temp)
                    });

                std::sort(res.begin(), res.end());
                return res;
            }
    };

    //****************************************************************
    // PointerMatrix
    //****************************************************************

    template<class T>
    class PointerMatrix {
        private:
            std::vector< std::vector< std::unique_ptr<T> > > m;
            int rows;
            int cols;

            class TPtrRef {
                private:
                    std::unique_ptr<T> &ptr;

                public:
                    TPtrRef(std::unique_ptr<T> &_ptr): ptr(_ptr) {}

                    const TPtrRef &operator=(T *new_ptr) {
                        ptr.reset(new_ptr);
                        return *this;
                    }

                    operator T*() {
                        return ptr.get();
                    }
            };

        public:
            static const int ROWS_MAX = 16384, COLS_MAX = 16384;

            PointerMatrix<T>(const PointerMatrix<T> &) = delete;
            PointerMatrix<T> &operator=(const PointerMatrix<T> &) = delete;

            PointerMatrix<T>(const int _rows, const int _cols, T *orig):
            m(), rows(_rows), cols(_cols) {
                if (rows <= 0 || cols <= 0 || rows > ROWS_MAX || cols > COLS_MAX)
                    throw std::invalid_argument("Incorrect bound numbers");
                if (!orig)
                    throw std::invalid_argument("Invalid original object");
                m.resize(rows);
                for (int i = 0; i < rows; i++) {
                    m[i].resize(cols);
                    for (int j = 0; j < cols; j++)
                        m[i][j] = std::unique_ptr<T>(orig->clone());
                }
            }

            ~PointerMatrix<T>() {}

            int get_rows() const {
                return rows;
            }

            int get_cols() const {
                return cols;
            }

            TPtrRef at(const int row, const int col) {
                if (row < 0 || col < 0 || row >= rows || col >= cols)
                    throw std::range_error("Range error");
                return TPtrRef(m[row][col]);
            }

            T *at(const int row, const int col) const {
                if (row < 0 || col < 0 || row >= rows || col >= cols)
                    throw std::range_error("Range error");
                return m[row][col].get();
            }
    };
};
cmcc
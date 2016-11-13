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
};

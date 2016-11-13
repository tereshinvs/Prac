#include <stdexcept>
#include <vector>
#include <new>
#include <memory>
#include <algorithm>

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
/*
class Obj {
    private:
        int a;

    public:
        Obj(const int _a): a(_a) {}
        ~Obj() {}

        virtual Obj* clone() const {
            return new Obj(*this);
        }
};

int main() {
    Obj obj(10);
    auto mat = new PointerMatrix<Obj>(5, 5, &obj);
    delete mat;
}*/
//
// Created by Adam Kepinski on 11/12/2020.
//

#ifndef ADVENTOFCODE_2020_MATRIX_H
#define ADVENTOFCODE_2020_MATRIX_H
#include <algorithm>
#include <iostream>
#include <istream>

template<typename T>
class Matrix {
    std::vector<T> data_;
    int columns_;
    int rows_;

    std::pair<int, int> posToRowColumn(int pos){
        return std::make_pair(pos / columns_, pos % columns_);
    }

public:
    explicit Matrix(std::istream& data_stream) {
        std::string row;
        rows_ = 0;
        while (std::getline(data_stream, row)) {
            std::copy(row.begin(), row.end(), std::back_inserter(data_));
            ++rows_;
        }
        columns_ = row.size();
    }

    T get(int row, int column) {
        assert(column < columns_);
        assert(row < rows_);
        return data_[row * columns_ + column];
    }

    void set(int row, int column, T value) {
        assert(column < columns_);
        assert(row < rows_);
        data_[row * columns_ + column] = value;
    }

    std::vector<T> getSurrounding(int row, int column) {
        std::vector<T> surrounding;
        for (int r = row - 1; r <= row+1; ++r) {
            if (r < 0) continue;
            if (r >= rows_) break;
            for (int c = column - 1; c <= column+1; ++c) {
                if (c < 0 || (r == row && c == column)) continue;
                if (c >= columns_) break;
                surrounding.push_back(get(r, c));
            }
        }
        return surrounding;
    }

    auto insert(typename std::vector<T>::iterator it, T value) {
        return data_.insert(it, value);
    }

    std::vector<T> getSurrounding(typename std::vector<T>::iterator it) {
        auto pos = it - data_.begin();
        auto [row, column] = posToRowColumn(pos);
        return getSurrounding(row, column);
    }

    std::vector<T> getVisibleSurrounding(typename std::vector<T>::iterator it, T invisible_element) {
        const auto pos = it - data_.begin();
        const auto [row, column] = posToRowColumn(pos);
        std::vector<T> visible_surrounding;

        // upper left
        for(auto row_ul = row - 1, column_ul = column - 1;
            row_ul >= 0 && column_ul >= 0; --row_ul, --column_ul) {
            auto elem = get(row_ul, column_ul);
            if (elem != invisible_element) {
                visible_surrounding.push_back(elem);
                break;
            }
        }

        // up
        for (auto row_u = row - 1, column_u = column; row_u >= 0; --row_u) {
            auto elem = get(row_u, column_u);
            if (elem != invisible_element) {
                visible_surrounding.push_back(elem);
                break;
            }
        }

        // upper right
        for (auto row_ur = row - 1, column_ur = column + 1; row_ur >= 0 && column_ur < columns_; --row_ur, ++column_ur) {
            auto elem = get(row_ur, column_ur);
            if (elem != invisible_element) {
                visible_surrounding.push_back(elem);
                break;
            }
        }

        // right
        for (auto row_r = row, column_r = column + 1; column_r < columns_; ++column_r) {
            auto elem = get(row_r, column_r);
            if (elem != invisible_element) {
                visible_surrounding.push_back(elem);
                break;
            }
        }

        // bottom right
        for (auto row_br = row + 1, column_br = column + 1; row_br < rows_ && column_br < columns_; ++row_br, ++column_br) {
            auto elem = get(row_br, column_br);
            if (elem != invisible_element) {
                visible_surrounding.push_back(elem);
                break;
            }
        }

        // bottom
        for (auto row_b = row + 1, column_b = column; row_b < rows_ && column_b < columns_; ++row_b) {
            auto elem = get(row_b, column_b);
            if (elem != invisible_element) {
                visible_surrounding.push_back(elem);
                break;
            }
        }

        // bottom left
        for (auto row_bl = row + 1, column_bl = column - 1; row_bl < rows_ && column_bl >= 0; ++row_bl, --column_bl) {
            auto elem = get(row_bl, column_bl);
            if (elem != invisible_element) {
                visible_surrounding.push_back(elem);
                break;
            }
        }

        // left
        for (auto row_l = row, column_l = column - 1; column_l >= 0; --column_l) {
            auto elem = get(row_l, column_l);
            if (elem != invisible_element) {
                visible_surrounding.push_back(elem);
                break;
            }
        }

        return visible_surrounding;
    }

    typename std::vector<T>::iterator begin() {
        return data_.begin();
    }

    typename std::vector<T>::iterator end() {
        return data_.end();
    }

    bool operator==(const Matrix<T>& other) {
        return columns_ == other.columns_ && rows_ == other.rows_ && data_ == other.data_;
    }

    bool operator!=(const Matrix<T>& other) {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        for (int pos = 0; pos < matrix.data_.size(); ++pos) {
            os << matrix.data_[pos];
            if (pos % matrix.columns_ == matrix.columns_ - 1)
                os << "\n";
        }

        return os;
    }
};

#endif //ADVENTOFCODE_2020_MATRIX_H

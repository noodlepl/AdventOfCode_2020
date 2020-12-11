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

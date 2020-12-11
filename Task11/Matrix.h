//
// Created by Adam Kepinski on 11/12/2020.
//

#ifndef ADVENTOFCODE_2020_MATRIX_H
#define ADVENTOFCODE_2020_MATRIX_H
#include <algorithm>
#include <iostream>

template<typename T>
class Matrix {
    std::vector<T> data_;
    int columns_;

public:
    Matrix(int columns) : columns_(columns) {
    }

    void addRow(const std::vector<T> row) {
        assert(row.size() == columns_);
        std::copy(row.begin(), row.end(), std::back_inserter(data_));
    }

    bool operator==(const Matrix<T>& other) {
        return columns_ == other.columns_ && data_ == other.data_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        for (int pos = 0; pos < data_.size(); ++pos) {
            os << data_[pos];
            if (pos % columns_ == 0)
                os << "\n"
        }

        return os;
    }
};

#endif //ADVENTOFCODE_2020_MATRIX_H

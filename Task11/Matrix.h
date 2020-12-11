//
// Created by Adam Kepinski on 11/12/2020.
//

#ifndef ADVENTOFCODE_2020_MATRIX_H
#define ADVENTOFCODE_2020_MATRIX_H
#include <algorithm>

template<typename T>
class Matrix {
    std::vector<T> data_;
    int rows_;
    int columns_;

public:
    Matrix(int rows, int columns) : rows_(rows), columns_(columns) {
        data_.reserve(rows * columns);
    }

    void addRow(const std::vector<T> row) {
        assert(row.size() == rows_);
        std::copy(row.begin(), row.end(), std::back_inserter(data_));
    }
};

#endif //ADVENTOFCODE_2020_MATRIX_H

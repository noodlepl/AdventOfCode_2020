//
// Created by Adam Kepinski on 07/12/2020.
//

#ifndef ADVENTOFCODE_2020_AREAMAP_H
#define ADVENTOFCODE_2020_AREAMAP_H
#include <vector>
#include <fstream>
#include <iostream>

template <class T>
class AreaMap {
public:
    class iterator {
        int x_step_;
        int y_step_;
        long long int x_;
        long long int y_;
        AreaMap<T>& container_;
    public:
        iterator(AreaMap<T>& container, int x_step, int y_step, int x = 0, int y = 0) : container_(container), x_step_(x_step), y_step_(y_step), x_(x), y_(y) {
            auto x_rem = x_ % x_step_;
            if (x_rem > 0) x_ += x_step_ - x_rem;
            auto y_rem = y_ % y_step_;
            if (y_rem > 0) y_ += y_step_ - y_rem;
            std::cout << "x_=" << x_ << ", y_=" << y_ << "\n";
        }
        iterator& operator++() {
            x_ += x_step_;
            y_ += y_step_;
            if (y_ >= container_.rows_) x_ = -1;
            return *this;
        }
        iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        bool operator==(iterator other) const {return y_ == other.y_ && x_ == other.x_;}
        bool operator!=(iterator other) const {return !(*this == other);}
        long operator*() {return container_.fields_[y_*container_.columns_ + x_ % container_.columns_];}
        // iterator traits
        using difference_type = long;
        using value_type = long;
        using pointer = const long*;
        using reference = const long&;
        using iterator_category = std::forward_iterator_tag;
    };
    AreaMap(const std::string& filename) {
        std::ifstream fs(filename);
        std::string line;

        auto output_it = std::back_inserter(fields_);

        rows_ = 0;
        while (std::getline(fs, line)) {
            std::copy(line.begin(), line.end(), output_it);
            ++rows_;
        }
        columns_ = line.size();
    }

    T get(int x, int y) {
        assert(y < rows_);

        return fields_[y*columns_ + x % columns_];
    }

    iterator begin(int x_step = 3, int y_step = 1) {return iterator(*this, x_step, y_step);}
    iterator end(int x_step = 3, int y_step = 1) {
        return iterator(*this, x_step, y_step, -1, rows_);
    }

private:
    std::vector<T> fields_;
    int columns_;
    int rows_;
};


#endif //ADVENTOFCODE_2020_AREAMAP_H

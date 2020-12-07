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
        const int x_step_ = 3;
        const int y_step_ = 1;
        int x_;
        int y_;
        AreaMap<T>& container_;
    public:
        iterator(AreaMap<T>& container, int x = 0, int y = 0) : container_(container), x_(x), y_(y) {
            auto x_rem = x_ % x_step_;
            if (x_rem > 0) x_ += x_step_ - x_rem;
            auto y_rem = y_ % y_step_;
            if (y_rem > 0) y_ += y_step_ - y_rem;
            std::cout << "x_: " << x_ << ", y_: " << y_ << "\n";
        }
        iterator& operator++() {
            x_ += x_step_;
            y_ += y_step_;
            y_ = std::min(y_, container_.rows_);
            return *this;
        }
        iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        bool operator==(iterator other) const {return y_ == other.y_ && x_%container_.columns_ == other.x_%container_.columns_;}
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

    iterator begin() {return iterator(*this);}
    iterator end() {
        return iterator(*this, columns_, rows_);
    }

private:
    std::vector<T> fields_;
    int columns_;
    int rows_;
};


#endif //ADVENTOFCODE_2020_AREAMAP_H

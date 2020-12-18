#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <sstream>
#include <cstdint>
#include <map>
#include <regex>
#include <array>
#include <list>
#include <cmath>

class Plane {
    int dimension_;
    std::deque<std::deque<char>> elements_;

public:
    Plane(int dimension) : dimension_(dimension){
        elements_ = std::deque<std::deque<char>>(dimension, std::deque<char>(dimension, '.'));
    }

    Plane(std::vector<char> input) {
        const auto input_size = input.size();
        dimension_ = sqrt(input_size);
        for (auto i = 0; i < dimension_; ++i) {
            std::deque<char> row;
            for (auto j = 0; j < dimension_; ++j) {
                row.push_back(input[i * dimension_ + j]);
            }
            elements_.push_back(row);
        }
    }

    char get(int x, int y) {
        int shift = elements_.size() / 2;
        assert(x >= -shift && x <= shift);
        assert(y >= -shift && y <= shift);

        return elements_[y + shift][x+shift];
    }

    void set(int x, int y, char value) {
        int shift = elements_.size() / 2;
        assert(x >= -shift && x <= shift);
        assert(y >= -shift && y <= shift);

        elements_[y+shift][x+shift] = value;
    }

    int getActiveCount(int x, int y) {
        int count = 0;
        for (int row = y - 1; row <= y + 1; ++row) {
            for (int col = x - 1; col <= x+1; ++col) {
                if (get(col, row) == '#') ++count;
            }
        }
        return count;
    }

    int getActiveExlusiveCount(int x, int y) {
        int count = 0;
        for (int row = y - 1; row <= y + 1; ++row) {
            for (int col = x - 1; col <= x+1; ++col) {
                if (row == y && col == x) continue;
                if (get(col, row) == '#') ++count;
            }
        }
        return count;
    }

    void grow() {
        for (auto&& row : elements_) {
            row.push_front('.');
            row.push_front('.');
            row.push_back('.');
            row.push_back('.');
        }
        dimension_ += 4;
        std::deque<char> empty_row(dimension_, '.');
        elements_.push_front(empty_row);
        elements_.push_front(empty_row);
        elements_.push_back(empty_row);
        elements_.push_back(empty_row);
    }

    int dimension() const {return dimension_;}

    int countActive() {
        int count = 0;
        for(auto&& row : elements_) {
            for (auto&& elem : row) {
                if (elem == '#') ++count;
            }
        }
        return count;
    }

    friend std::ostream& operator<<(std::ostream& os, const Plane& plane) {
        os << "Plane size " << plane.dimension_ << " size of elements: " << plane.elements_.size() << "\n";
        for (auto&& row : plane.elements_) {
            for(auto elem : row) {
                os << elem;
            }
            os << "\n";
        }
        return os;
    }
};

class Cube {
    std::deque<Plane> planes_;

    friend std::ostream& operator<<(std::ostream& os, const Cube& cubes) {
        for (auto&& plane : cubes.planes_) {
            os << plane << "\n";
        }
        return os;
    }

public:
    Cube(std::vector<char> input) {
        int dimension = sqrt(input.size());
        planes_.emplace_back(dimension);
        planes_.emplace_back(std::move(input));
        planes_.emplace_back(dimension);
    }

    char get(int x, int y, int z) {
        const int shift = planes_.size() / 2;
        assert(z >= -shift && z <= shift);
        return planes_[z + shift].get(x, y);
    }

    void grow() {
        for (auto&& plane : planes_) {
            plane.grow();
        }
        auto dimension = planes_[0].dimension();

        planes_.emplace_back(dimension);
        planes_.emplace_front(dimension);
    }

    void runCycle() {
        Cube next_cube(*this);
        for (auto z = 1; z < planes_.size() - 1; ++z) {
            const auto dimension = planes_[z].dimension();
            const auto boundary = dimension/2 - 1;
            for (int row = -boundary; row <= boundary; ++row) {
                for(int col = -boundary; col <= boundary; ++col) {
                    auto area_0 = planes_[z].getActiveExlusiveCount(col, row);
                    auto area_minus_1 = planes_[z-1].getActiveCount(col, row);
                    auto area_plus_1 = planes_[z+1].getActiveCount(col, row);
                    auto total_active = area_0 + area_minus_1 + area_plus_1;

                    if(planes_[z].get(col, row) == '#') {
                        if (total_active != 2 && total_active != 3)
                            next_cube.planes_[z].set(col, row, '.');
                    } else if (total_active == 3) {
                        next_cube.planes_[z].set(col, row, '#');
                    }
                }
            }
        }
        std::swap(*this, next_cube);
        grow();
    }

    int countActive() {
        int count = 0;
        for(auto&& plane : planes_) {
            count += plane.countActive();
        }
        return count;
    }
};

Cube readData() {
    std::ifstream ifs("input.txt");
    std::string line;
    std::vector<char> cubes_input;
    while (std::getline(ifs, line)) {
        for (auto c : line)
            cubes_input.push_back(c);
    }

    return Cube(cubes_input);
}


int main() {
    auto start = std::chrono::system_clock::now();

    auto cubes = readData();
    cubes.grow();
    std::cout << cubes << "\n";

    for (int i = 0; i < 6; ++i) {
        cubes.runCycle();
    }
//    std::cout << cubes << "\n";
    std::cout << "Active cubes: " << cubes.countActive() << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
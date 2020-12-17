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
#include <math.h>

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

    friend std::ostream& operator<<(std::ostream& os, const Plane& plane) {
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
        planes_.emplace_back(dimension);
        planes_.emplace_back(std::move(input));
        planes_.emplace_back(dimension);
        planes_.emplace_back(dimension);
    }

    char get(int x, int y, int z) {
        const int shift = planes_.size() / 2;
        assert(z >= -shift && z <= shift);
        return planes_[z + shift].get(x, y);
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
    std::cout << cubes << "\n";

    std::cout << "Test cubes: " << cubes.get(0,0,0) << "," << cubes.get(1, 0, 0) << "," << cubes.get(-1,-1,-1) << ","
    << cubes.get(1,1,2);

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
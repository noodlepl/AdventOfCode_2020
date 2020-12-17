#include <iostream>
#include <fstream>
#include <vector>
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

class Cubes {
    std::vector<char> cubes_;
    int planes_count_ = 5;
    int plane_dimmension_;

    int dimSquared() const {
        return plane_dimmension_*plane_dimmension_;
    }

    void resize() {

    }

    friend std::ostream& operator<<(std::ostream& os, const Cubes& cubes) {
        for (int i = 0; i < cubes.cubes_.size(); ++i) {
            if (i % (cubes.plane_dimmension_) == 0)
                os << "\n";
            if (i % (cubes.dimSquared()) == 0)
                os << "\n";
            os << cubes.cubes_[i];
        }
        return os;
    }

public:
    Cubes(std::vector<char> input) {
        auto input_size = input.size();
        plane_dimmension_ = sqrt(input_size);
        cubes_ = std::vector<char>(plane_dimmension_*plane_dimmension_*planes_count_, '.');
        std::copy(input.begin(), input.end(), cubes_.begin() + 2*plane_dimmension_*plane_dimmension_);
    }

    char get(int x, int y, int z) {
        const auto planes_shift = planes_count_ / 2;
        const auto single_plane_shift = plane_dimmension_ / 2;
        assert(x >= -single_plane_shift && x <= single_plane_shift);
        assert(y >= -single_plane_shift && y <= single_plane_shift);
        assert(z >= -planes_shift && z <= planes_shift);

        const auto pos = (z + planes_shift) * dimSquared() + (y + single_plane_shift) * plane_dimmension_ + (x + single_plane_shift);
        return cubes_[pos];
    }
};

Cubes readData() {
    std::ifstream ifs("input.txt");
    std::string line;
    std::vector<char> cubes_input;
    while (std::getline(ifs, line)) {
        for (auto c : line)
            cubes_input.push_back(c);
    }

    return Cubes(cubes_input);
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
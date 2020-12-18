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
#include <unordered_set>

struct Coordinate {
    int x;
    int y;
    int z;
    int w;
    friend bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
    }
};

struct CoordHash
{
    std::size_t operator()(Coordinate const& coordinate) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(coordinate.x);
        std::size_t h2 = std::hash<int>{}(coordinate.y);
        std::size_t h3 = std::hash<int>{}(coordinate.z);
        std::size_t h4 = std::hash<int>{}(coordinate.w);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};

using Cubes = std::unordered_set<Coordinate, CoordHash>;

std::pair<Cubes, int> readData() {
    std::ifstream ifs("input.txt");
    std::string line;
    Cubes active_cubes;
    std::vector<char> input;
    int dimension = 0;
    while (std::getline(ifs, line)) {
        std::copy(line.begin(), line.end(), std::back_inserter(input));
        ++dimension;
    }

    auto it = input.begin();
    auto start_value = -dimension/2;
    auto end_value = start_value + dimension;

    for (int row = start_value; row < end_value; ++row) {
        for (int col =  start_value; col < end_value; ++col) {
            if(*it == '#') {
                auto [iterator, success] = active_cubes.insert(Coordinate{col, row, 0, 0});
                assert(success);
            }
            ++it;
        }
    }

    return std::make_pair(active_cubes, dimension);
}

void printCubes(const Cubes& cubes, const Coordinate& start_coord) {
    auto [start_x, start_y, start_z, start_w] = start_coord;
    assert(start_x <= 0 && start_y <= 0 && start_z <= 0);
    for (int hyper = start_w; hyper <= -start_w; ++hyper) {
        for (int plane = start_z; plane <= -start_z; ++plane) {
            std::cout << "z=" << plane << ", w=" << hyper <<"\n";
            for (int row = start_y; row <= -start_y; ++row) {
                for (int col = start_x; col <= -start_x; ++col) {
                    if (std::find(cubes.begin(), cubes.end(), Coordinate{col, row, plane, hyper}) != cubes.end()) {
                        std::cout << '#';
                    } else {
                        std::cout << '.';
                    }
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
    }
}

bool activate(const Cubes& cubes, const Coordinate& coord) {
    auto [col,row,plane,hyper] = coord;
    auto active_count = 0;
    for (auto x = col-1; x <= col+1; ++x){
        for (auto y = row-1; y <= row+1; ++y){
            for (auto z = plane-1; z <= plane+1; ++z){
                for (auto w = hyper-1; w <=hyper+1; ++w) {
                    if (x == col && y == row && z == plane && w == hyper) continue;
                    if (cubes.find(Coordinate{x, y, z, w}) != cubes.end())
                        ++active_count;
                }
            }
        }
    }
    bool is_active = cubes.find(coord) != cubes.end();
    if (is_active && (active_count == 2 || active_count == 3))
        return true;
    if (!is_active && active_count == 3)
        return true;

    return false;
}

Cubes runCycle(Cubes current, const Coordinate& start_coord) {
    Cubes next;
    auto [start_x, start_y, start_z, start_w] = start_coord;
    assert(start_x <= 0 && start_y <= 0 && start_z <= 0 && start_w <=0);

    for (auto x = start_x; x <= -start_x; ++x) {
        for (auto y = start_y; y <= -start_y; ++y) {
            for (auto z = start_z; z <= -start_z; ++z) {
                for (auto w = start_w; w <= -start_w; ++w)
                    if (activate(current, Coordinate{x,y,z,w}))
                        next.insert(Coordinate{x,y,z,w});
            }
        }
    }

    return next;
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto [cubes, dimension] = readData();

//    printCubes(cubes, Coordinate{-4, -4, 0, 0});

    Coordinate start_coord{-dimension/2, -dimension/2, 0, 0};
    for (int i = 0; i < 6; ++i){
        start_coord = {--start_coord.x, --start_coord.y, --start_coord.z, --start_coord.w};
        cubes = runCycle(cubes, start_coord);
    }

//    printCubes(cubes, Coordinate{-4, -4, -0});
    std::cout << "Result: " << cubes.size() << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
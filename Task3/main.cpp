#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "AreaMap.h"

std::pair<int, int> getDataDimensions() {
    std::ifstream fs("input.txt", std::ios_base::in);
    std::string s;
    int count = 0;
    while (std::getline(fs, s)) {
        ++count;
    }
    return std::make_pair(count, s.size());
}

std::vector<int> readData(int rows, int columns) {
    std::ifstream fs("input.txt", std::ios_base::in);
    std::vector<int> data;

    return data;
}

int main() {
    auto start = std::chrono::system_clock::now();

    AreaMap<char> areaMap("input.txt");

    int trees_count = std::count_if(areaMap.begin(), areaMap.end(), [](char a){return a == '#';});

    std::cout << "trees count: " << trees_count << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}

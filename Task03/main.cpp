#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>

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

    auto pred = [](char a){return a == '#';};
    long long int trees_count_1_1 = std::count_if(areaMap.begin(1, 1), areaMap.end(1, 1), pred);
    std::cout << "trees_count_1_1: " << trees_count_1_1 << "\n";
    long long int trees_count_3_1 = std::count_if(areaMap.begin(3, 1), areaMap.end(3, 1), pred);
    std::cout << "trees_count_3_1: " << trees_count_3_1 << "\n";
    long long int trees_count_5_1 = std::count_if(areaMap.begin(5, 1), areaMap.end(5, 1), pred);
    std::cout << "trees_count_5_1: " << trees_count_5_1 << "\n";
    long long int trees_count_7_1 = std::count_if(areaMap.begin(7, 1), areaMap.end(7, 1), pred);
    std::cout << "trees_count_7_1: " << trees_count_7_1 << "\n";
    long long int trees_count_1_2 = std::count_if(areaMap.begin(1, 2), areaMap.end(1, 2), pred);
    std::cout << "trees_count_1_2: " << trees_count_1_2 << "\n";

    long long int result = trees_count_1_1*trees_count_3_1*trees_count_5_1*trees_count_7_1*trees_count_1_2;
    std::cout << "trees count multiplied: " << result << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}

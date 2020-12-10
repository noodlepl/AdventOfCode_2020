#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <cstdint>

std::vector<int> readData() {
    std::ifstream ifs("input.txt", std::ios_base::in);
    int x;
    std::vector<int> data;

    while (ifs >> x)
        data.push_back(x);

    return data;
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto data = readData();

    data.push_back(0);
    std::sort(data.begin(), data.end());
    data.push_back(data.back() + 3);
    for (auto&& num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << "Length: " << data.size() << "\n";

    auto prev = data.begin();
    auto ones_count = 0;
    auto threes_count = 0;

    for (auto current = prev + 1; current != data.end(); ++current, ++prev) {
        auto diff = *current - *prev;
        if (diff == 1)
            ++ones_count;
        else if (diff == 3)
            ++threes_count;
    }

    std::cout << "1 jolts diffs: " << ones_count << ", 3 jolts diffs: " << threes_count << ", multiplication: " << ones_count * threes_count << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
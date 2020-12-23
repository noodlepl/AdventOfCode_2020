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

int main() {
    auto start = std::chrono::system_clock::now();

    std::ifstream ifs("input.txt");
    std::string input_line;
    std::getline(ifs, input_line);
    std::vector<int> cups;
    for (auto c : input_line)
        cups.push_back(c - '0');

    constexpr int rounds_count = 100;
    auto source_it = cups.begin();
    for (int i = 0; i < rounds_count; ++i) {
//        std::cout << "-- move " << i << "--\ncups: ";
//        for (auto cup : cups)
//            std::cout << cup << ",";
//        std::cout << "\npick up: ";
        auto rotate_begin = std::next(source_it);
        auto rotate_end = rotate_begin + 3;
//        for (auto it = rotate_begin; it != rotate_end; ++it) {
//            std::cout << *it << ",";
//        }
        auto destination_it = cups.end();
        auto destination_value = *source_it - 1;
        while (destination_it == cups.end()) {
            destination_it = std::find(rotate_end, cups.end(), destination_value);
            if (--destination_value <= 0)
                destination_value = *std::max_element(rotate_end, cups.end());
        }
//        std::cout << "\ndestination: " << *destination_it << "\n";
        // rotate 3 elements;
        std::rotate(rotate_begin, rotate_end, destination_it + 1);
        // keep source at the left
        std::rotate(cups.begin(), cups.begin() + 1, cups.end());
    }

    std::cout << "\n-- final --\n";
    for (auto cup : cups)
        std::cout << cup;
    auto one_it = std::find(cups.begin(), cups.end(), 1);
    std::rotate(cups.begin(), one_it, cups.end());
    std::cout << "\nResult: ";
    for (auto it = cups.begin() + 1;it != cups.end();++it) {
        std::cout << *it;
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "\nProgram duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
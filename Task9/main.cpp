#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <cstdint>

std::vector<int64_t> readData() {
    std::ifstream ifs("input.txt", std::ios_base::in);
    std::vector<int64_t> data;
    int64_t number;
    while (ifs >> number) {
        data.push_back(number);
    }
    return data;
}

int64_t getFirstInvalid(const std::vector<int64_t>& data, int preamble_length) {
    auto main_it = data.begin() + preamble_length;

    auto first_invalid = std::find_if_not(main_it, data.end(), [&main_it, preamble_length](auto number) {
        bool valid = false;
        for (auto it1 = main_it - preamble_length; it1 != main_it; ++it1) {
            for (auto it2 = it1 + 1; it2 != main_it; ++it2) {
                if (*it1 != *it2 && *it1 + *it2 == number) {
                    valid = true;
                    break;
                }
            }
            if (valid) break;
        }
        ++main_it;
        return valid;
    });

    return *first_invalid;
}

int64_t findWeakness(const std::vector<int64_t> data, int64_t invalid_number) {
    auto invalid_it = std::find(data.begin(), data.end(), invalid_number);
    if (invalid_it == data.end()) return -1;
    auto it1 = data.begin();
    auto it2 = it1 + 1;
    bool found_interval = false;

    for (; it1 != invalid_it; ++it1) {
        int64_t sum = *it1;
        for (it2 = it1 + 1;it2 != invalid_it; ++it2) {
            sum += *it2;
            if (sum > invalid_number) break;
            if (sum == invalid_number) {
                found_interval = true;
                break;
            }
        }
        if (found_interval) break;
    }
    auto max = std::max_element(it1, it2 + 1);
    auto min = std::min_element(it1, it2 + 1);

    return *min + *max;
}

int main() {
    auto start = std::chrono::system_clock::now();

    //Part 1
    auto data = readData();
    auto first_invalid = getFirstInvalid(data, 25);
    std::cout << "First invalid: " << first_invalid << std::endl;

    //Part2
    auto weakness = findWeakness(data, first_invalid);
    std::cout << "Weakness: " << weakness << std::endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
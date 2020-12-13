#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <cstdint>

using Input = std::pair<int64_t, std::vector<int64_t>>;

Input readData() {
    std::ifstream ifs("input.txt");
    int64_t my_estimate;
    ifs >> my_estimate;

    std::string bus_ids_string;
    ifs >> bus_ids_string;
    std::istringstream iss(bus_ids_string);
    std::string interval;
    std::vector<int64_t> bus_ids;

    while(std::getline(iss, interval, ',')) {
        if (interval != "x")
            bus_ids.push_back(std::stoi(interval));
    }

    return std::make_pair(my_estimate, bus_ids);
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto [my_estimate, bus_ids] = readData();
    std::cout << "My estimate: " << my_estimate << "\n" << "Bus intervals: ";
    for (auto id : bus_ids) {
        std::cout << id << ",";
    }
    std::cout << std::endl;
    auto estimate_copy = my_estimate;
    decltype(bus_ids) transformed_ids;

    std::transform(bus_ids.begin(), bus_ids.end(), std::back_inserter(transformed_ids), [estimate_copy](auto id) {
        int divide = estimate_copy/id;
        int multiply = id * (divide + 1);
        return multiply;
    });
    std::cout << "Transformed: ";
    for (auto id : transformed_ids) {
        std::cout << id << ",";
    }
    std::cout << std::endl;
    auto min_it = std::min_element(transformed_ids.begin(), transformed_ids.end());
    auto min_position = min_it - transformed_ids.begin();

    auto result = bus_ids[min_position] * (*min_it - my_estimate);
    std::cout << "Result: " << result << std::endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
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

using Input2 = std::vector<std::pair<int64_t, int64_t>>;
Input2 readData2(){
    std::ifstream ifs("input.txt");
    std::string line;
    std::getline(ifs, line); //ignore
    std::getline(ifs, line);

    std::istringstream iss(line);
    std::string id;
    Input2 values;
    int64_t pos = 0;
    while(std::getline(iss, id, ',')){
        if (id != "x")
            values.push_back(std::make_pair(std::stoi(id), pos));
        ++pos;
    }

    return values;
}

int main() {
    auto start = std::chrono::system_clock::now();

//    auto [my_estimate, bus_ids] = readData();
//    std::cout << "My estimate: " << my_estimate << "\n" << "Bus intervals: ";
//    for (auto id : bus_ids) {
//        std::cout << id << ",";
//    }
//    std::cout << std::endl;
//    //Part 1
//    auto estimate_copy = my_estimate;
//    decltype(bus_ids) transformed_ids;
//
//    std::transform(bus_ids.begin(), bus_ids.end(), std::back_inserter(transformed_ids), [estimate_copy](auto id) {
//        int divide = estimate_copy/id;
//        int multiply = id * (divide + 1);
//        return multiply;
//    });
//    std::cout << "Transformed: ";
//    for (auto id : transformed_ids) {
//        std::cout << id << ",";
//    }
//    std::cout << std::endl;
//    auto min_it = std::min_element(transformed_ids.begin(), transformed_ids.end());
//    auto min_position = min_it - transformed_ids.begin();
//
//    auto result = bus_ids[min_position] * (*min_it - my_estimate);
//    std::cout << "Result: " << result << std::endl;

    //Part 2:
    Input2 input2 = readData2();
    int64_t max = std::accumulate(input2.begin(), input2.end(), 1, [](auto a, const auto& b) {
       return a * b.first;
    });
    std::cout << "max: " << max << "\n";

//    std::sort(input2.begin(), input2.end(), [](auto a, auto b){return a.first < b.first;});
    std::transform(input2.begin(), input2.end(), input2.begin(), [](auto a) {
        auto remainder = a.first - ((a.first + a.second) % a.first);
        if (remainder == a.first) remainder = 0;
        return std::make_pair(a.first, remainder);
    });
    std::cout << "Values: ";
    for (auto elem : input2) {
        std::cout << elem.first << " ";
    }
    std::cout << "\n";
    std::cout << "Remainders: ";
    for (auto&& elem : input2) {
        std::cout << elem.second << " ";
    }
    std::cout << "\n";

    int64_t solution;
    auto it = input2.begin();
    auto divisor = it->first;
    auto remainder = it->second;

    for (; it != input2.end()-1; ++it) {
        auto next = std::next(it);
        for (int i = 0; ; ++i) {
            solution = divisor * i + remainder;
            if (solution % next->first == next->second)
                break;
        }
        divisor *= next->first;
        remainder = solution;
    }

    std::cout << "solution: " << solution << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
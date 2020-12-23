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
#include <unordered_map>

std::vector<int> readData() {
    std::ifstream ifs("input.txt");
    std::string input_line;
    std::getline(ifs, input_line);
    std::vector<int> cups;
    for (auto c : input_line)
        cups.push_back(c - '0');
    return cups;
}

std::list<int> readDataAsList() {
    std::ifstream ifs("input.txt");
    std::string input_line;
    std::getline(ifs, input_line);
    std::list<int> cups;
    for (auto c : input_line)
        cups.push_back(c - '0');
    return cups;
}

int64_t part2Result(std::list<int> cups_list) {
    auto original_size = cups_list.size();
    auto list_start = *cups_list.begin();
    auto list_end = cups_list.back();
    std::unordered_map<int, int> cups;
    for (auto it = cups_list.begin(); it != cups_list.end(); ++it) {
        auto next = std::next(it);
        if (next == cups_list.end()) next = cups_list.begin();

        cups[*it] = *next;
    }

    constexpr int max_value = 1000000;
    cups[list_end] = original_size+1;
    for (int i = original_size + 1; i <=max_value; ++i) {
        auto next = (i == max_value) ? list_start : i+1;
        cups[i] = next;
    }

    std::cout << "size: " << cups.size() << " after 8 goes " << cups[8] << " and then " << cups[cups[8]]
    << " after 1000000 goes " << cups[max_value] << std::endl;

    constexpr int iterations_count = 10000000;
    for (int i = 0, pos = list_start; i < iterations_count; ++i, pos = cups[pos]) {
        std::array<int, 3> pick_up{cups[pos], cups[cups[pos]], cups[cups[cups[pos]]]};

        int destination = pos - 1;
        bool is_in_pick_up = std::find(pick_up.begin(), pick_up.end(), destination) != pick_up.end();
        while (is_in_pick_up || destination < 1) {
            destination -= 1;
            if (destination < 1) destination = max_value;
            is_in_pick_up = std::find(pick_up.begin(), pick_up.end(), destination) != pick_up.end();
        }

        cups[pos] = cups[pick_up[2]];
        cups[pick_up[2]] = cups[destination];
        cups[destination] = pick_up[0];
    }

//    for (int j = cups[1]; j != 1; j = cups[j]) {
//        std::cout << j;
//    }
//    std::cout << std::endl;
    int64_t first_star = cups[1];
    int64_t second_star = cups[first_star];
    std::cout << "first: " << first_star << " second star: " << second_star << "\n";

    return first_star * second_star;
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto cups = readData();

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

//    std::cout << "\n-- final --\n";
//    for (auto cup : cups)
//        std::cout << cup;
    auto one_it = std::find(cups.begin(), cups.end(), 1);
    std::rotate(cups.begin(), one_it, cups.end());
//    std::cout <<"/Result part 2: " << static_cast<int64_t>(cups[1]) * static_cast<int64_t>(cups[2]) << "\n";
//    std::cout << "\nResult: ";
//    for (auto it = cups.begin() + 1;it != cups.end();++it) {
//        std::cout << *it;
//    }

    auto resultPart2 = part2Result(readDataAsList());
    std::cout << "Result part 2: " << resultPart2;

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "\nProgram duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
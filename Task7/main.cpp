#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <optional>
#include <set>
#include <deque>

namespace {
    const std::string kBagsContain = "bags contain";

    std::vector<std::string> readData() {
        std::ifstream ifs("input.txt", std::ios_base::in);
        std::string line;
        std::vector<std::string> instructions;

        while (std::getline(ifs, line)) {
            instructions.push_back(line);
        }

        return instructions;
    }

    std::optional<std::string> canFitBag(const std::string &instruction, const std::string &bag_type) {
        auto bags_contain_pos = instruction.find(kBagsContain);
        if (bags_contain_pos == std::string::npos)
            throw std::invalid_argument("instruction is not valid");

        auto bag_type_pos = instruction.find(bag_type, bags_contain_pos + kBagsContain.size());
        if (bag_type_pos == std::string::npos)
            return std::nullopt;

        auto container_bag_type = instruction.substr(0, bags_contain_pos - 1);

        return container_bag_type;
    }
}
int main() {
    auto start = std::chrono::system_clock::now();

    auto instructions = readData();
    std::for_each(instructions.begin(), instructions.end(), [](auto instruction){std::cout << instruction << "\n";});

    int single_run_bags_count = 0;
    int total_valid_bags_count = 0;
    std::deque<std::string> valid_types = {"shiny gold"};
    std::set<std::string> checked_types;

    do {
        single_run_bags_count = std::count_if(instructions.begin(), instructions.end(), [&valid_types, &checked_types](auto instruction){
            auto bag_type = valid_types.front();
            valid_types.pop_front();
            auto new_bag_type = canFitBag(instruction, bag_type);
            if (new_bag_type && std::find(checked_types.begin(), checked_types.end(), *new_bag_type) == checked_types.end()) {

                valid_types.push_back(*new_bag_type);
            }
            checked_types.insert(bag_type);
            return new_bag_type != std::nullopt;
        });

        total_valid_bags_count += single_run_bags_count;
    } while (single_run_bags_count > 0);

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}

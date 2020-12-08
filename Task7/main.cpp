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

    std::optional<std::string> canFitBag(const std::string &instruction, const std::string &bag_color) {
        auto bags_contain_pos = instruction.find(kBagsContain);
        if (bags_contain_pos == std::string::npos)
            throw std::invalid_argument("instruction is not valid");

        auto bag_color_pos = instruction.find(bag_color, bags_contain_pos + kBagsContain.size());
        if (bag_color_pos == std::string::npos)
            return std::nullopt;

        auto container_bag_color = instruction.substr(0, bags_contain_pos - 1);

        return container_bag_color;
    }
}
int main() {
    auto start = std::chrono::system_clock::now();

    auto instructions = readData();
    std::for_each(instructions.begin(), instructions.end(), [](auto instruction){std::cout << instruction << "\n";});

    std::deque<std::string> colors_to_check = {"shiny gold"};
    std::set<std::string> valid_colors;
    std::set<std::string> checked_colors;

    do {
        std::count_if(instructions.begin(), instructions.end(), [&colors_to_check, &checked_colors, &valid_colors](auto instruction){
            auto bag_color = colors_to_check.front();
            auto new_bag_color = canFitBag(instruction, bag_color);
            if (new_bag_color && std::find(checked_colors.begin(), checked_colors.end(), *new_bag_color) == checked_colors.end()) {
                colors_to_check.push_back(*new_bag_color);
                valid_colors.insert(*new_bag_color);
            }
            checked_colors.insert(bag_color);
            return new_bag_color != std::nullopt;
        });
        colors_to_check.pop_front();
    } while (!colors_to_check.empty());

    std::cout << "Number of colors that can contain shiny gold bag: " << valid_colors.size() << std::endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <optional>
#include <set>
#include <deque>
#include <unordered_map>
#include <regex>

namespace {
    const std::string kBagsContain = "bags contain";

    std::vector<std::string> readDataAsVector() {
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

    struct InstructionDetail {
        int number;
        std::string bag_color;
    };

    using InstructionsMap = std::unordered_map<std::string, std::vector<InstructionDetail>>;
    InstructionsMap::value_type parseInstruction(const std::string& instruction) {
        auto bags_contain_pos = instruction.find(kBagsContain);
        if (bags_contain_pos == std::string::npos)
            throw std::invalid_argument("instruction is not valid");

        auto key = instruction.substr(0, bags_contain_pos - 1);
        std::cout << "key: " << key << "\n";

        std::regex r("([0-9]+) ([a-z]+ [a-z]+) bag[s]?");
        std::smatch match_results;
        auto instruction_details = instruction.substr(bags_contain_pos + kBagsContain.size() + 1);
        std::vector<InstructionDetail> value;
        std::cout << "Detailed instr: " << instruction_details << "\n";

        while(std::regex_search(instruction_details, match_results, r)) {
            assert(match_results.size() == 3);
            value.push_back({std::stoi(match_results[1]), match_results[2]});
            instruction_details = match_results.suffix();
        }

        return {key, value};
    }

    InstructionsMap readDataAsMap() {
        std::ifstream ifs("input.txt", std::ios_base::in);
        std::string line;
        InstructionsMap instructions;

        while(std::getline(ifs, line)) {
            instructions.insert(parseInstruction(line));
        }

        return instructions;
    }

    long long int countBagsInside(const InstructionsMap& map, const InstructionsMap::key_type& key) {
        auto instruction_details = map.at(key);
        long long int result = 0;
        for (auto&& detail : instruction_details) {
            auto bags_inside = countBagsInside(map, detail.bag_color);
            result += detail.number * bags_inside;
        }

        return result + 1;
    }
}

int main() {
    auto start = std::chrono::system_clock::now();
    // Part 2
    auto instructions = readDataAsMap();

    long long int num_bags_in_shiny_gold = countBagsInside(instructions, "shiny gold") - 1;
    std::cout << "Bags in shiny gold: " << num_bags_in_shiny_gold << std::endl;

//    for (auto&& [key, value]: instructions) {
//        std::cout << "[" << key << ":(\n";
//        for(auto&& detail : value) {
//            std::cout << "\t{" << detail.number << ", " << detail.bag_color << "}\n";
//        }
//        std::cout << ")]\n";
//    }

// Part 1
//    auto instructions = readDataAsVector();
//    std::for_each(instructions.begin(), instructions.end(), [](auto instruction){std::cout << instruction << "\n";});
//
//    std::deque<std::string> colors_to_check = {"shiny gold"};
//    std::set<std::string> valid_colors;
//    std::set<std::string> checked_colors;
//
//    do {
//        std::count_if(instructions.begin(), instructions.end(), [&colors_to_check, &checked_colors, &valid_colors](auto instruction){
//            auto bag_color = colors_to_check.front();
//            auto new_bag_color = canFitBag(instruction, bag_color);
//            if (new_bag_color && std::find(checked_colors.begin(), checked_colors.end(), *new_bag_color) == checked_colors.end()) {
//                colors_to_check.push_back(*new_bag_color);
//                valid_colors.insert(*new_bag_color);
//            }
//            return new_bag_color != std::nullopt;
//        });
//        checked_colors.insert(colors_to_check.front());
//        colors_to_check.pop_front();
//    } while (!colors_to_check.empty());
//
//    std::cout << "Number of colors that can contain shiny gold bag: " << valid_colors.size() << std::endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}

//int main() {
//    std::string lines[] = {"Roses are #ff0000",
//                           "violets are #0000ff",
//                           "all of my base are belong to you"};
//
//    std::regex color_regex("#([a-f0-9]{2})"
//                           "([a-f0-9]{2})"
//                           "([a-f0-9]{2})");
//
//    // simple match
//    for (const auto &line : lines) {
//        std::cout << line << ": " << std::boolalpha
//                  << std::regex_search(line, color_regex) << '\n';
//    }
//    std::cout << '\n';
//
//    // show contents of marked subexpressions within each match
//    std::smatch color_match;
//    for (const auto& line : lines) {
//        if(std::regex_search(line, color_match, color_regex)) {
//            std::cout << "matches for '" << line << "'\n";
//            std::cout << "Prefix: '" << color_match.prefix() << "'\n";
//            for (size_t i = 0; i < color_match.size(); ++i)
//                std::cout << i << ": " << color_match[i] << '\n';
//            std::cout << "Suffix: '" << color_match.suffix() << "\'\n\n";
//        }
//    }
//
//    // repeated search (see also std::regex_iterator)
//    std::string log(R"(
//        Speed:	366
//        Mass:	35
//        Speed:	378
//        Mass:	32
//        Speed:	400
//	Mass:	30)");
//    std::regex r(R"(Speed:\t\d*)");
//    std::smatch sm;
//    while(regex_search(log, sm, r))
//    {
//        std::cout << sm.str() << '\n';
//        log = sm.suffix();
//    }
//
//    // C-style string demo
//    std::cmatch cm;
//    if(std::regex_search("this is a test", cm, std::regex("test")))
//        std::cout << "\nFound " << cm[0] << " at position " << cm.prefix().length();
//}

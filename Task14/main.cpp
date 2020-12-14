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

using bitval = std::bitset<36>;
bitval g_mask_or;
bitval g_mask_and = ~g_mask_or;
std::map<int64_t, bitval> address_space_;
std::map<uint64_t, uint64_t> g_address_space_2;

struct {
    bitval mask;
    std::vector<int> x_positions;
} g_string_mask;

void updateMasks(const std::string& mask) {
    std::string delim = "= ";
    auto mask_bits_pos = mask.find(delim);
    auto mask_bits = mask.substr(mask_bits_pos + delim.size());

    std::string mask_or;
    std::transform(mask_bits.begin(), mask_bits.end(), std::back_inserter(mask_or), [](auto a){
        if (a == 'X') a = '0';
        return a;
    });
    g_mask_or = bitval(mask_or);

    std::string mask_and;
    std::transform(mask_bits.begin(), mask_bits.end(), std::back_inserter(mask_and), [](auto a){
        if (a == 'X') a = '1';
        return a;
    });
    g_mask_and = bitval(mask_and);
}

void assignValue(const std::string& instruction) {
    std::regex r("^mem\\[([0-9]+)\\] = ([0-9]+)$");
    std::smatch match;
    std::regex_search(instruction, match, r);
    auto key = std::stoull(match[1]);
    auto value = std::stoull(match[2]);
    bitval value_bits(value);
    value_bits &= g_mask_and;
    value_bits |= g_mask_or;
    address_space_[key] = value_bits;
}

void executeLinePart1(const std::string& line) {
    if (line.find("mask") != std::string::npos)
        updateMasks(line);
    else if(line.find("mem") != std::string::npos)
        assignValue(line);
}

void updateStringMask(std::string line) {
    std::string delim = "= ";
    line = line.substr(line.find(delim) + delim.size());
    g_string_mask.x_positions.clear();

    std::for_each(line.begin(), line.end(), [index = line.size()](auto a) mutable {
        --index;
        if (a == 'X')
            g_string_mask.x_positions.push_back(index);
    });
    std::transform(line.begin(), line.end(), line.begin(), [](auto elem) {
        if (elem == 'X')
            elem = '0';
        return elem;
    });
    std::reverse(g_string_mask.x_positions.begin(), g_string_mask.x_positions.end());
    g_string_mask.mask = bitval(line);
    std::cout << g_string_mask.mask << "\n";
    for (auto&& val : g_string_mask.x_positions) {
        std::cout << val << ",";
    }
    std::cout << "\n";
}

void applyToAllAdresses(std::string line) {
    std::regex r("^mem\\[([0-9]+)\\] = ([0-9]+)$");
    std::smatch match;
    std::regex_search(line, match, r);
    auto base_key = std::stoull(match[1]);
    auto value = std::stoull(match[2]);
    bitval mask(g_string_mask.mask);
    auto address_count = 1 << g_string_mask.x_positions.size();
    auto base_key_bits = bitval(base_key);
    base_key_bits |= g_string_mask.mask;
    for (auto pos : g_string_mask.x_positions)
        base_key_bits.reset(pos);
//    std::cout << "base_key_bits 0: " << base_key_bits << "\n";
    g_address_space_2[base_key_bits.to_ullong()] = value;
    for (int i = 1; i < address_count; ++i) {
        auto j = i;
        int pos = 0;
        do {
            auto divisor = 1 << pos;
            if(i % divisor == 0)
                base_key_bits.flip(g_string_mask.x_positions[pos++]);
        } while (j /= 2);
//        std::cout << "Base key bits " << i << ": " << base_key_bits << "\n";
        g_address_space_2[base_key_bits.to_ulong()] = value;
    }
//    for (auto&& kv : g_address_space_2) {
//        std::cout << "[" << kv.first << "," << kv.second << "]\n";
//    }
}

void executeLinePart2(std::string line) {
    if (line.find("mask") != std::string::npos)
        updateStringMask(std::move(line));
    else if(line.find("mem") != std::string::npos)
        applyToAllAdresses(std::move(line));
}

int main() {
    auto start = std::chrono::system_clock::now();

    // Part 1
//    std::ifstream ifs("input.txt", std::ios_base::in);
//    std::string line;
//    while (std::getline(ifs, line)) {
//        executeLinePart1(line);
//    }
//

    // Part 2
    std::ifstream ifs("input.txt", std::ios_base::in);
    std::string line;
    while(std::getline(ifs, line)) {
        executeLinePart2(std::move(line));
    }

//    auto sum = std::accumulate(address_space_.begin(), address_space_.end(), 0ULL, [](auto sum, auto&& element) {
//        auto value = element.second.to_ullong();
//        return sum + value;
//    });
    auto sum = std::accumulate(g_address_space_2.begin(), g_address_space_2.end(), 0ULL, [](auto sum, auto&& element) {
        return sum + element.second;
    });

    std::cout << "Result: " << sum << "\n";
    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
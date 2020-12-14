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
//    std::cout << "Inserting: " << value_bits << "\n";
    address_space_[key] = value_bits;
}

void executeLine(const std::string& line) {
    if (line.find("mask") != std::string::npos)
        updateMasks(line);
    else if(line.find("mem") != std::string::npos)
        assignValue(line);
}

int main() {
    auto start = std::chrono::system_clock::now();

    std::ifstream ifs("input.txt", std::ios_base::in);
    std::string line;
    while (std::getline(ifs, line)) {
        executeLine(line);
    }

    auto sum = std::accumulate(address_space_.begin(), address_space_.end(), 0ULL, [](auto sum, auto&& element) {
        auto value = element.second.to_ullong();
        return sum + value;
    });

    std::cout << "Result: " << sum << "\n";
    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
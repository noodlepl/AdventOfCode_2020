#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <sstream>
#include <cstdint>
#include <map>

using bitval = std::bitset<36>;
bitval g_mask_or;
bitval g_mask_and = ~g_mask_or;
std::map<int64_t, bitval> address_space_;

void updateMasks(const std::string& mask) {
    std::string mask_or;
    std::transform(mask.begin(), mask.end(), std::back_inserter(mask_or), [](auto a){
        if (a == 'X') a = '0';
        return a;
    });
    g_mask_or = bitval(mask_or);

    std::string mask_and;
    std::transform(mask.begin(), mask.end(), std::back_inserter(mask_and), [](auto a){
        if (a == 'X') a = '1';
        return a;
    });
    g_mask_and = bitval(mask_and);
}

void assignValue(const std::string& instruction) {
    bitval bit_instr(instruction);
}

void executeLine(const std::string& line) {

}

int main() {
    auto start = std::chrono::system_clock::now();

    std::string input_mask("0110010011X1X1X0000000XX0110000XX111");
    updateMasks(input_mask);
    std::cout << "input mask:\t" << input_mask << "\nmask OR:\t" << g_mask_or << "\nmask AND:\t" << g_mask_and << "\n";

    std::ifstream ifs("input.txt", std::ios_base::in);
    std::string line;
    while (std::getline(ifs, line)) {
        executeLine(line);
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
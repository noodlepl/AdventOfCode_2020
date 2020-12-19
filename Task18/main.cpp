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
#include <cctype>

namespace {
    constexpr char kPlus = '+';
    constexpr char kTimes = '*';
    constexpr char kOpenBracket = '(';
    constexpr char kCloseBracket = ')';
}
void handleBrackets(std::string& line);
int64_t calculate(std::string line);

void handleBrackets(std::string& line) {
    for (int open_pos = line.find(kOpenBracket); open_pos != std::string::npos; open_pos = line.find(kOpenBracket, open_pos + 1)) {
        int close_pos = line.find(kCloseBracket);
        if (close_pos == std::string::npos) {
            assert(false);
        }
        for (int next_open_pos = open_pos; next_open_pos < close_pos;) {
            next_open_pos = line.find(kOpenBracket, next_open_pos + 1);
            if (next_open_pos == std::string::npos)
                break;
            if (next_open_pos < close_pos) {
                close_pos = line.find(kCloseBracket, close_pos+1);
                assert(close_pos != std::string::npos);
            }
        }

        auto result = calculate(line.substr(open_pos + 1, close_pos - open_pos - 1));
        line.erase(open_pos, close_pos - open_pos+1);
        line.insert(open_pos, std::to_string(result));
    }
}

void handleAdditions(std::string& line) {
    for (auto operator_pos = line.find(kPlus); operator_pos != std::string::npos; operator_pos = line.find(kPlus, operator_pos)) {
        auto operand1_pos = line.rfind(kTimes, operator_pos-1);
        if (operand1_pos == std::string::npos)
            operand1_pos = 0;
        else
            ++operand1_pos;

        auto operand1 = std::stoll(line.substr(operand1_pos, operator_pos));
        auto next_op_pos = line.find_first_of("*+", operator_pos + 1);
        auto operand2 = std::stoll(line.substr(operator_pos+1, next_op_pos));
        int64_t result = operand1 + operand2;
        line.erase(operand1_pos, next_op_pos - operand1_pos);
        line.insert(operand1_pos, std::to_string(result));
    }
}

int64_t calculate(std::string line) {
    handleBrackets(line);
    handleAdditions(line);
    auto operator_pos = line.find("*");
    int64_t result;
    if (operator_pos != std::string::npos) {
        auto operand1 = std::stoll(line.substr(0, operator_pos));
        auto next_op_pos = line.find("*", operator_pos + 1);
        auto operand2 = std::stoll(line.substr(operator_pos+1, next_op_pos));
        result = operand1 * operand2;
        if (next_op_pos != std::string::npos){
            line.erase(0, next_op_pos);
            line.insert(0, std::to_string(result));
            result = calculate(std::move(line));
        }
    } else {
        result = std::stoll(line);
    }
    return result;
}

int main() {
    auto start = std::chrono::system_clock::now();

    std::ifstream ifs("input.txt");
    std::string line;

    int64_t sum = 0;
    while (std::getline(ifs, line)) {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        std::cout << line << " = ";
        auto result = calculate(std::move(line));
        std::cout << result << "\n";
        sum+=result;
    }

    std::cout << "Sum: " << sum << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
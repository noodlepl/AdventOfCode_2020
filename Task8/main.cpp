#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>

class Instruction {
public:
    Instruction (const std::string& instruction_string) {
        constexpr char delimiter = ' ';
        std::istringstream iss(instruction_string);
        std::getline(iss, type, delimiter);
        std::string value_str;
        std::getline(iss, value_str, delimiter);
        value = std::stoi(value_str);
    }

    friend std::ostream& operator<<(std::ostream& os, const Instruction& instruction) {
        os << instruction.type << ' ' << instruction.value;
        return os;
    }

    std::string type;
    int value;
};

using Instructions = std::vector<Instruction>;

std::vector<Instruction> readData() {
    std::ifstream ifs("input.txt", std::ios_base::in);
    std::string line;
    std::vector<Instruction> instructions;

    while(std::getline(ifs, line)) {
        instructions.emplace_back(line);
    }

    return instructions;
}

int execute(const Instructions& instructions, int instr_index, int& accumulated_value) {
    const Instruction& instr = instructions[instr_index];

    if (instr.type == "nop") {
        return ++instr_index;
    }
    if (instr.type == "acc") {
        accumulated_value += instr.value;
        return ++instr_index;
    }
    if (instr.type == "jmp") {
        return (instr_index + instr.value);
    }

    throw std::invalid_argument("unknown instruction");
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto instructions = readData();

    std::vector<int> executed_indexes;
    int accumulated_value = 0;
    int current_index = 0;

    do {
        executed_indexes.push_back(current_index);
        current_index = execute(instructions, current_index, accumulated_value);
    } while (std::find(executed_indexes.begin(), executed_indexes.end(), current_index) == executed_indexes.end());

    std::cout << "Accumulated value: " << accumulated_value << std::endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
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

void switchType(std::string& type) {
    if (type == "nop") type = "jmp";
    if (type == "jmp") type = "nop";
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto original_instructions = readData();

    //Part 1
//    std::vector<int> executed_indexes;
//    int accumulated_value = 0;
//    int current_index = 0;
//
//    do {
//        executed_indexes.push_back(current_index);
//        current_index = execute(original_instructions, current_index, accumulated_value);
//    } while (std::find(executed_indexes.begin(), executed_indexes.end(), current_index) == executed_indexes.end());
//
//    std::cout << "Accumulated value in part 1: " << accumulated_value << std::endl;

    //Part 2
    for (int i = 0; i < original_instructions.size(); ++i) {
        Instruction& instr = original_instructions[i];
        if (instr.type == "acc") continue;
        Instructions fixed_instructions(original_instructions);
        switchType(fixed_instructions[i].type);

        std::vector<int> executed_indexes;
        int accumulated_value = 0;
        int current_index = 0;
        bool fixed = false;

        do {
            executed_indexes.push_back(current_index);
            current_index = execute(fixed_instructions, current_index, accumulated_value);
            if (current_index >= fixed_instructions.size()) {
                fixed = true;
                break;
            }
        } while (std::find(executed_indexes.begin(), executed_indexes.end(), current_index) == executed_indexes.end());
        if (fixed) {
            std::cout << "Accumulated value in part 2: " << accumulated_value << std::endl;
            break;
        }
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
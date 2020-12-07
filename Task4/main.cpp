#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include "Passport.h"

std::string readDataAsString() {
    std::ifstream ifs("input.txt", std::ios_base::in);
    std::ostringstream oss;
    std::string line;
    while (std::getline(ifs, line)) {
        oss << line;
        if (ifs.good()) oss << '\n';
    }

    return oss.str();
}

int countValidPassports(std::string input) {
    std::string delimiter = "\n\n";

    auto pos = 0;
    auto count = 0;
    while ((pos = input.find(delimiter)) != std::string::npos) {
        auto passport_input = input.substr(0, pos);
        Passport passport(std::move(passport_input));
        if (passport.isValid()) ++count;
        input.erase(0, pos + delimiter.size());
    }

    return count;
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto data = readDataAsString();

    auto result = countValidPassports(data);

    std::cout << "Valid passports: " << result << std::endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}

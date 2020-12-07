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

    auto count = 0;
    auto count_valid = 0;

    for (auto pos = input.find(delimiter); pos != std::string::npos; pos = input.find(delimiter)) {
        auto passport_input = input.substr(0, pos);
        Passport passport(std::move(passport_input));
        if (passport.isValid()) ++count_valid;
        input.erase(0, pos + delimiter.size());
        ++count;
    }
    // parse the last input
    Passport passport(std::move(input));
    if (passport.isValid()) ++count_valid;
    ++count;

    std::cout << "Processed passports " << count << std::endl;
    return count_valid;
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

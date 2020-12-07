#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>

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

int main() {
    auto start = std::chrono::system_clock::now();

    auto data = readDataAsString();

    std::cout << data << std::endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}

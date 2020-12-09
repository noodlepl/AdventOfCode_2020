#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

int main() {
    auto start = std::chrono::system_clock::now();


    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
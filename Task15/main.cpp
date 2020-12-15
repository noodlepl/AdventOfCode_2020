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

int main() {
    auto start = std::chrono::system_clock::now();

    std::array<int, 6> input = {2,0,6,12,1,3};
    std::vector<int> game(input.begin(), input.end());
    const int limit = 2020;
    for (int i = input.size(); i < limit; ++i) {
        auto& last_element = game.back();
        auto last_pos = std::find(game.rbegin() + 1, game.rend(), last_element);
        if (last_pos == game.rend())
            game.push_back(0);
        else {
            auto distance = last_pos - game.rbegin();
            game.push_back(distance);
        }
//        std::cout << "Spoken number: " << game.back() << "\n";
    }

    std::cout << "Result: " << game.back() << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
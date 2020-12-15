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
#include <unordered_map>

int main() {
    auto start = std::chrono::system_clock::now();

    std::array<int, 6> input = {2,0,6,12,1,3};
//    std::vector<int> game(input.begin(), input.end());
//    const int limit = 2020;
//    for (int i = input.size(); i < limit; ++i) {
//        auto& last_element = game.back();
//        auto last_pos = std::find(game.rbegin() + 1, game.rend(), last_element);
//        if (last_pos == game.rend())
//            game.push_back(0);
//        else {
//            auto distance = last_pos - game.rbegin();
//            game.push_back(distance);
//        }
////        std::cout << "Spoken number: " << game.back() << "\n";
//    }

//    std::cout << "Result: " << game.back() << "\n";

    std::vector<int64_t> long_game(input.begin(), input.end());
    std::unordered_map<int64_t, int64_t> last_positions;
    std::for_each(long_game.begin(), long_game.end() - 1, [&last_positions, index=0](auto elem) mutable {
        last_positions[elem] = index++;
    });

    int64_t new_limit = 30000000;
    for (int64_t i = long_game.size(); i < new_limit; ++i) {
        auto last_element = long_game.back();
        if (last_positions.find(last_element) == last_positions.end())
            long_game.push_back(0);
        else
            long_game.push_back(i - last_positions[last_element] - 1);
        last_positions[last_element] = i - 1;
    }

    std::cout << "Result: " << long_game.back() << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
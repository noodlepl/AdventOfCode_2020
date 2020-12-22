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

bool is_number(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}
using Deck = std::deque<int>;
std::pair<Deck, Deck> readData() {
    Deck deck1;
    Deck deck2;
    auto* working_deck = &deck1;
    std::ifstream ifs("input.txt");
    std::string line;

    while (std::getline(ifs, line)) {
        if (line.empty())
            working_deck = &deck2;
        if (!is_number(line)) continue;
        working_deck->push_back(std::stoi(line));
    }

    return {deck1, deck2};
}

void passCard(Deck& from, Deck& to) {
    auto to_it = to.begin();
    std::rotate(to_it, std::next(to_it), to.end());
    to.push_back(from.front());
    from.pop_front();
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto [deck1, deck2] = readData();

    int count = 0;
    do {
        auto top_card_1 = deck1.front();
        auto top_card_2 = deck2.front();

        if (top_card_1 > top_card_2)
            passCard(deck2, deck1);
        else
            passCard(deck1, deck2);

        std::cout << "Round " << count++ << "\n";
    } while(!deck1.empty() && !deck2.empty());

    auto& winning_deck = deck1.empty() ? deck2 : deck1;
    int multiplier = 1;
    int result = std::accumulate(winning_deck.rbegin(), winning_deck.rend(), 0, [&multiplier](auto sum, auto current) {
        return sum + (multiplier++) * current;
    });

    std::cout << "Result: " << result << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
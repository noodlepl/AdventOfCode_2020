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
#include <set>

bool is_number(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}
using Deck = std::deque<int>;
std::array<Deck, 2> readData() {
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

void printCards(const Deck& deck) {
    int count = 0;
    for (auto card : deck) {
        if (count > 0)
            std::cout << ",";
        std::cout << card;
        count++;
    }
    std::cout << "\n";
}

int playGame(std::array<Deck, 2>& decks) {
    int count = 1;
    static int game_count = 0;
    ++game_count;
    std::set<Deck> ref_decks0;
    ref_decks0.insert(decks[0]);
    std::set<Deck> ref_decks1;
    ref_decks1.insert(decks[1]);

    int winner;
    do {
        std::cout << "--Round " << count << " Game " << game_count << " --\n";
        std::cout << "Player 0's deck: ";
        printCards(decks[0]);
        std::cout << "Player 1's deck: ";
        printCards(decks[1]);
        if (count > 1) {
            auto [it0, success0] = ref_decks0.insert(decks[0]);
            auto [it1, success1] = ref_decks1.insert(decks[1]);
            if (!success0 && !success1) {
                std::cout << "Repetition, player 0 wins\n";
                return 0;
            }
        }

        auto top_card_0 = decks[0].front();
        std::cout << "Player 0 plays " << top_card_0 << "\n";
        auto top_card_1 = decks[1].front();
        std::cout << "Player 1 plays " << top_card_1 << "\n";

        if (top_card_0 <= decks[0].size() - 1 && top_card_1 <= decks[1].size() - 1) {
            auto deck0_it = decks[0].begin() + 1;
            auto deck1_it = decks[1].begin() + 1;
            std::array<Deck, 2> subdecks = {Deck(deck0_it, deck0_it+top_card_0),
                                            Deck(deck1_it, deck1_it+top_card_1)};
            std::cout << "Playing a subgame to determine the winner...\n";
            winner = playGame(subdecks);
        } else {
            winner = top_card_0 > top_card_1 ? 0 : 1;
            std::cout << "Player " << winner << " wins round " << count << " of game " << game_count << "\n";
        }
        passCard(decks[!winner], decks[winner]);
        ++count;
    } while(!decks[0].empty() && !decks[1].empty());

    return winner;
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto decks = readData();


    auto winner = playGame(decks);

    int multiplier = 1;
    int result = std::accumulate(decks[winner].rbegin(), decks[winner].rend(), 0, [&multiplier](auto sum, auto current) {
        return sum + (multiplier++) * current;
    });

    std::cout << "Result: " << result << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
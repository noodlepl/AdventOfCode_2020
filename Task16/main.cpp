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

using Rules = std::unordered_map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>>;
using Ticket = std::vector<int>;
Rules g_rules;
Ticket g_my_ticket;
std::vector<Ticket> g_other_tickets;

void addRule(std::string line) {
    std::regex r("^(.*): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)$");
    std::smatch results;
    std::regex_search(line, results, r);
    assert(results.size() == 6);
    auto key = results[1];
    auto first_range = std::make_pair(std::stoi(results[2]), std::stoi(results[3]));
    auto second_range = std::make_pair(std::stoi(results[4]), std::stoi(results[5]));
    auto [it, result] = g_rules.insert({key, std::make_pair(first_range, second_range)});
    assert(result);
}

Ticket readTicket(std::string line) {
    std::istringstream iss(line);
    std::string token;
    auto delimiter = ',';
    Ticket ticket;
    while(std::getline(iss, token, delimiter))
        ticket.push_back(std::stoi(token));
    return ticket;
}

void readData() {
    std::ifstream ifs("input.txt");
    std::string line;
    int state = 0;
    while(std::getline(ifs, line)) {
        std::cout << line << "\n";
        if (line.empty()) continue;
        else if(line == "your ticket:") {
            state = 1;
            continue;
        }
        else if(line == "nearby tickets:") {
            state = 2;
            continue;
        }

        switch (state) {
            case 0:
                addRule(std::move(line));
                break;
            case 1:
                g_my_ticket = readTicket(std::move(line));
                break;
            case 2:
                g_other_tickets.push_back(readTicket(std::move(line)));
                break;
        }
    }

}

int main() {
    auto start = std::chrono::system_clock::now();

    readData();

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
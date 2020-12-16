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

using Rules = std::map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>>;
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

    //merge rules;
    std::list<std::pair<int, int>> merged_rules;
    std::for_each(g_rules.begin(), g_rules.end(), [&merged_rules](const auto& rule) {
        merged_rules.push_back(rule.second.first);
        merged_rules.push_back(rule.second.second);
    });
    merged_rules.sort([](const auto& a, const auto& b){
        return a.first < b.first;
    });
    for (auto it = std::next(merged_rules.begin()); it != merged_rules.end();) {
        auto prev = std::prev(it);
        if (it->first <= prev->second + 1) {
            prev->second = it->second;
            it = merged_rules.erase(it);
        } else
            ++it;
    }

    for (auto&& elem : merged_rules){
        std::cout << "[" << elem.first << "," << elem.second << "] ";
    }
    std::cout << std::endl;

    int64_t sum = 0;
    for (auto&& ticket : g_other_tickets) {
        for (auto&& val : ticket) {
            bool valid = false;
            for (auto&& range : merged_rules) {
                if (val >= range.first && val <= range.second) {
                    valid = true;
                    break;
                }
            }
            if (!valid)
                sum += val;
        }
    }

    std::cout << "Result: " << sum << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
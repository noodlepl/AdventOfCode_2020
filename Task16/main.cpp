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

}

Ticket readTicket(std::string) {

}

void readData() {
    std::ifstream ifs("intput.txt");
    std::string line;
    int state = 0;
    while(std::getline(ifs, line)) {
        

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


    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
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
#include <unordered_map>
#include <memory>

std::unordered_map<int, std::string> g_rules;
std::vector<std::string> g_messages;

struct Node {
    Node(int l) : level(l){}
    int level;
    std::unique_ptr<Node> a = nullptr;
    std::unique_ptr<Node> b = nullptr;
};

void readRule(std::string line) {
    auto key_end = line.find(':');
    auto [it, success] = g_rules.insert({std::stoi(line.substr(0, key_end)), line.substr(key_end + 1)});
    assert(success);
}

std::string decodeRule(int);

std::string decodeRecursiveRule(int key) {
    auto rule = g_rules[key];
    auto pos = rule.find('|');
    rule.erase(pos, std::string::npos);
    std::ostringstream oss;
    std::istringstream iss(rule);
    int next_key;
    while (iss >> next_key) {
        oss << "(?:";
        oss << decodeRule(next_key);
        oss << ")+";
    }
    return oss.str();
}

std::string decode8() {
    std::ostringstream oss;
    oss << "(" << decodeRule(42) << "|" << "(?:" << decodeRule(42) << ")+" << ")";
    return oss.str();
}

std::string decode11() {
    std::ostringstream oss;
    oss << "(" << decodeRule(42) << decodeRule(31) << "|" << decodeRule(42) << "(?:" << decodeRule(42) << ")+"<<"(?:"<<decodeRule(31)<<")+" << decodeRule(31) << ")";
    return oss.str();
}

std::string decodeRule(int key) {
    auto rule = g_rules[key];
    std::regex single_char_regex("\"([ab])\"");
    std::smatch mresult;
    if (std::regex_search(rule, mresult, single_char_regex))
        return mresult[1];

    if (key == 8)
        return decode8();
    if (key == 11)
        return decode11();
    const auto delimiter = '|';
    std::istringstream iss(rule);
    std::string token;
    std::ostringstream decoded_rule;
    decoded_rule << "(";
    while (std::getline(iss, token, delimiter)) {
        int next_key;
        std::istringstream internal_iss(token);
        while(internal_iss >> next_key) {
            if (next_key == key) {
//                decoded_rule << decodeRecursiveRule(next_key);
            } else {
                decoded_rule << decodeRule(next_key);
            }
        }
        if (iss.peek() == EOF) {
            decoded_rule << ')';
        } else
            decoded_rule << '|';
    }

    return decoded_rule.str();
}

int main() {
    auto start = std::chrono::system_clock::now();

    std::ifstream ifs("input.txt");
    std::string line;
    auto state = 0;
    while(std::getline(ifs, line)) {
        if (line.empty()){
            state = 1;
            continue;
        }
        if (state == 0)
            readRule(std::move(line));
        else if (state == 1)
            g_messages.push_back(std::move(line));
    }

    auto rule = decodeRule(0);
    std::ostringstream oss;
    oss << '^' << rule << '$';
    std::regex r(oss.str());
    std::cout << oss.str() << "\n";
    int count = 0;
    for (auto&& message : g_messages) {
        if (std::regex_match(message, r)) {
//            std::cout << message << "\n";
            ++count;
        }
    }

    std::cout << "Valid messages: " << count << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
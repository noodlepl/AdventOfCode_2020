#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <set>

void readData(){
    std::ifstream ifs("input.txt", std::ios_base::in);
    std::string line;
    std::set<char> group_answers;
    std::vector<int> yes_counts;
    std::string group_delimiter = "\n";

    while (std::getline(ifs, line)){
        std::copy(line.begin(), line.end(), std::back_inserter(group_answers));

        if (line == group_delimiter) {
            yes_counts.push_back(group_answers.size());
            group_answers.clear();
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

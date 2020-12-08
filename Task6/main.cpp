#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <set>

void handleGroup(std::set<char>& group, std::vector<int>& yes_counts) {
    std::cout << "Group answers: ";
    std::for_each(group.begin(), group.end(), [](auto a){std::cout << a;});
    std::cout << "\n";
    yes_counts.push_back(group.size());
    group.clear();
}

void countAnswers(){
    std::ifstream ifs("input.txt", std::ios_base::in);
    std::string line;
    std::set<char> group_answers;
    std::vector<int> yes_counts;

    while (std::getline(ifs, line)){
        std::copy(line.begin(), line.end(), std::inserter(group_answers, group_answers.begin()));

        if (line.empty()) {
            handleGroup(group_answers, yes_counts);
        }
    }
    handleGroup(group_answers, yes_counts);

    int sum = 0;
    std::for_each(yes_counts.begin(), yes_counts.end(), [&sum](auto i){sum += i;});
    std::cout << "Unique \"yeses\": " << sum << std::endl;
}

int main() {
    auto start = std::chrono::system_clock::now();

    countAnswers();

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}

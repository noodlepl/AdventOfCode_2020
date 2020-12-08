#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <set>

void handleGroupPart1(std::set<char>& group, std::vector<int>& yes_counts) {
    std::cout << "Group answers: ";
    std::for_each(group.begin(), group.end(), [](auto a){std::cout << a;});
    std::cout << "\n";
    yes_counts.push_back(group.size());
    group.clear();
}

void countAnswersPart1(){
    std::ifstream ifs("input.txt", std::ios_base::in);
    std::string line;
    std::set<char> group_answers;
    std::vector<int> yes_counts;

    while (std::getline(ifs, line)){
        std::copy(line.begin(), line.end(), std::inserter(group_answers, group_answers.begin()));

        if (line.empty()) {
            handleGroupPart1(group_answers, yes_counts);
        }
    }
    handleGroupPart1(group_answers, yes_counts);

    int sum = 0;
    std::for_each(yes_counts.begin(), yes_counts.end(), [&sum](auto i){sum += i;});
    std::cout << "Unique \"yeses\": " << sum << std::endl;
}

void handleGroupPart2(std::vector<std::string>& group_answers, std::vector<int>& yeses_count) {
    std::set<char> common_answers;
    std::for_each(group_answers.begin(), group_answers.end(), [&common_answers, &group_answers](auto answers) {
        std::copy_if(answers.begin(), answers.end(), std::inserter(common_answers, common_answers.begin()), [&group_answers](auto a){
            return std::all_of(group_answers.begin(), group_answers.end(), [a](auto answer){
                return answer.find(a) != std::string::npos;
            });
        });
    });
    std::cout << "Everyone answered: ";
    std::for_each(common_answers.begin(), common_answers.end(), [](auto a){std::cout << a;});
    std::cout << "\n";
    yeses_count.push_back(common_answers.size());

    group_answers.clear();
}

void countAnswersPart2() {
    std::ifstream ifs("input.txt", std::ios_base::in);
    std::string line;
    std::vector<std::string> group_answers;
    std::vector<int> yeses_count;

    while (std::getline(ifs, line)) {
        if (line.empty()) {
            handleGroupPart2(group_answers, yeses_count);
            continue;
        }
        group_answers.push_back(line);
    }
    handleGroupPart2(group_answers, yeses_count);

    int sum = 0;
    std::for_each(yeses_count.begin(), yeses_count.end(), [&sum](auto i){sum += i;});
    std::cout << "Unique \"yeses\": " << sum << std::endl;
}

int main() {
    auto start = std::chrono::system_clock::now();

    countAnswersPart2();

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}

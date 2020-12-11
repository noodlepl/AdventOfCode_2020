#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <cstdint>

std::vector<int> readData() {
    std::ifstream ifs("input.txt", std::ios_base::in);
    int x;
    std::vector<int> data;

    while (ifs >> x)
        data.push_back(x);

    return data;
}

bool transitionValid(const std::vector<int>& data, int current_id, int prev_id) {
    return (prev_id >= 0) && (data[current_id] - data[prev_id] <= 3);
}

template <typename Num>
Num countSequences(const std::vector<int>& data) {
    std::vector<Num> sequence_counts{1};

    for (int i = 1; i < data.size(); ++i) {
        int64_t ends_with_i_count = sequence_counts[i-1];
        if (transitionValid(data, i, i-2)) ends_with_i_count += sequence_counts[i-2];
        if (transitionValid(data, i, i-3)) ends_with_i_count += sequence_counts[i-3];

        sequence_counts.push_back(ends_with_i_count);
    }

    return sequence_counts.back();
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto data = readData();

    data.push_back(0);
    std::sort(data.begin(), data.end());
    data.push_back(data.back() + 3);
    for (auto&& num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << "Length: " << data.size() << "\n";

    // Part 1
    auto prev = data.begin();
    auto ones_count = 0;
    auto threes_count = 0;

    for (auto current = prev + 1; current != data.end(); ++current, ++prev) {
        auto diff = *current - *prev;
        if (diff == 1)
            ++ones_count;
        else if (diff == 3)
            ++threes_count;
    }

    std::cout << "1 jolts diffs: " << ones_count << ", 3 jolts diffs: " << threes_count << ", multiplication: " << ones_count * threes_count << "\n";

    // Part 2
    auto valid_sequences_count = countSequences<int64_t>(data);
    std::cout << "Valid sequence count: " << valid_sequences_count << std::endl;

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
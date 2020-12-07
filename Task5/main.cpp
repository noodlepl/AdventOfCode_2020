#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <optional>

template <int FROM, int TO>
class BinaryRange {
    int lower_bound_ = FROM;
    int upper_bound_ = TO;

    static_assert(FROM <= TO, "lower bound has to be smaller or equal than upper bound");

public:
    void goLower() {
        if (lower_bound_ != upper_bound_)
            upper_bound_ -= (upper_bound_ - lower_bound_) / 2 + 1;
    }

    void goUpper() {
        if (lower_bound_ != upper_bound_)
            lower_bound_ += (upper_bound_ - lower_bound_) / 2 + 1;
    }

    std::optional<int> getValue() {
        if (lower_bound_ == upper_bound_) {
            return lower_bound_;
        }
        return std::nullopt;
    }
};

std::pair<int, int> getRowAndColumn(const std::string& seat_code) {
    BinaryRange<0, 127> rows;
    BinaryRange<0, 7> columns;

    std::for_each(seat_code.begin(), seat_code.end(), [&rows, &columns](char c){
        switch(c) {
            case 'F':
                rows.goLower();
                break;
            case 'B':
                rows.goUpper();
                break;
            case 'L':
                columns.goLower();
                break;
            case 'R':
                columns.goUpper();
                break;
            default:
                std::ostringstream ss;
                ss << "Couldn't parse character: " << c;
                throw std::invalid_argument(ss.str());
        }
    });


    return {*rows.getValue(), *columns.getValue()};
}

void readData() {
    std::ifstream ifs("input.txt", std::ios_base::in);
    std::string line;
    long max_seat_id = 0;
    std::vector<long> seat_ids;

    while(std::getline(ifs, line)) {
        auto [row, column] = getRowAndColumn(line);
        long seat_id = row * 8 + column;
        seat_ids.push_back(seat_id);
//        std::cout << line << " = [" << row << ", " << column << "], ID = " << seat_id << "\n";
        max_seat_id = std::max(seat_id, max_seat_id);
    }
    std::cout << "Max Seat ID: " << max_seat_id << "\n";

    std::sort(seat_ids.begin(), seat_ids.end());
    long my_seat_id = -1;
    auto it = seat_ids.begin();
    for (auto next = it+1; next != seat_ids.end(); ++it, ++next) {
        if (*next - *it == 2) {
            my_seat_id = *it + 1;
            break;
        }
    }
    std::for_each(seat_ids.begin(), seat_ids.end(), [](long seat_id){std::cout << seat_id << "\n";});
    std::cout << "my seat ID: " << my_seat_id << "\n";
}

int main() {
    auto start = std::chrono::system_clock::now();

    readData();

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}

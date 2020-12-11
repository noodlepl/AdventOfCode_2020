#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <cstdint>
#include "Matrix.h"

Matrix<char> readData() {
    std::ifstream ifs("input.txt");
    Matrix<char> matrix(ifs);

    return matrix;
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto next_matrix = readData();
    auto original_matrix = next_matrix;
    std::cout << next_matrix << "\n";

    do {
        original_matrix = next_matrix;
        auto original_it = original_matrix.begin();
        auto next_it = next_matrix.begin();
        for(; original_it != original_matrix.end(); ++original_it, ++next_it) {
            switch(*original_it) {
                case '.':
                    break;
                case 'L':
                {
                    auto surrounding = original_matrix.getSurrounding(original_it);
                    auto occupied_it = std::find(surrounding.begin(), surrounding.end(), '#');
                    if (occupied_it == surrounding.end()) {
                        *next_it = '#';
                    }
                    break;
                }
                case '#':
                {
                    auto surrounding = original_matrix.getSurrounding(original_it);
                    auto occupied_count = std::count(surrounding.begin(), surrounding.end(), '#');
                    if (occupied_count >= 4) {
                        *next_it = 'L';
                    }
                    break;
                }
            }
        }
//        std::cout << next_matrix << "\n";
    } while (next_matrix != original_matrix);

    auto occupied_seats = std::count(original_matrix.begin(), original_matrix.end(), '#');
    std::cout << "There are: " << occupied_seats << " occupied seats.\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
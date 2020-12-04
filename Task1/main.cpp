#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<int> readData() {
    std::ifstream fs("input", std::ios_base::in);
    std::vector<int> data;
    int x = 0;
    while (fs.good()){
        fs >> x;
        data.push_back(x);
    }
    return data;
}

int main() {
    std::vector<int> data = readData();

    int result = -1;
    for(auto it1 = data.begin(); it1 != data.end(); ++it1) {
        for (auto it2 = it1 + 1; it2 != data.end(); ++it2) {
            if (*it1 + *it2 == 2020) {
                std::cout << "Number 1: " << *it1 << ", Number 2: " << *it2 << std::endl;
                result = *it1 * *it2;
                break;
            }
        }
        if (result != -1) {
            break;
        }
    }

    std::cout << result;
    return 0;
}

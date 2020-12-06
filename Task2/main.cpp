#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Password.h"

int countValidPasswords() {
    std::ifstream fs("input.txt", std::ios_base::in);
    std::string line;
    std::vector<int> data;
    int count = 0;

    while(std::getline(fs, line)) {
        Password pass(line);
        if (pass.isValid()) ++count;
    }

    return count;
}

int main() {
    std::cout << countValidPasswords();
    return 0;
}

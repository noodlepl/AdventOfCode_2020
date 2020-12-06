//
// Created by Adam Kepinski on 06/12/2020.
//

#include "Password.h"

#include <algorithm>

Password::Password(std::string input) {
    int pos = input.find("-");
    lower_bound_ = std::stoi(input.substr(0, pos));
    input.erase(0, pos + 1);

    pos = input.find(" ");
    upper_bound_ = std::stoi(input.substr(0, pos));
    input.erase(0, pos + 1);

    pos = input.find(":");
    required_character_ = input.at(pos - 1);
    input.erase(0, pos + 1);

    password_ = input.substr(input.find_first_not_of(" "));
}

bool Password::isValid() {
    auto count = std::count(password_.begin(), password_.end(), required_character_);

    return count >= lower_bound_ && count <= upper_bound_;
}
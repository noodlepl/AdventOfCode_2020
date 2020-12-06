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
    auto char_at_lower = password_.at(lower_bound_ - 1);
    auto char_at_upper = password_.at(upper_bound_ - 1);

    return char_at_lower != char_at_upper && (char_at_lower == required_character_ || char_at_upper == required_character_);
}
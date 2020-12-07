//
// Created by Adam Kepinski on 07/12/2020.
//

#include "Passport.h"

#include <iostream>
#include <regex>

namespace {
    bool validateByr(const std::string& value) {
        bool result = false;

        if (value.size() == 4) {
            try {
                int year = std::stoi(value);
                result = year >= 1920 && year <= 2002;
            } catch (...) {
                std::cout << "Couldn't convert " << value << " to number";
            }
        }

        return result;
    }

    bool validateIyr(const std::string& value) {
        bool result = false;

        if (value.size() == 4) {
            try {
                int year = std::stoi(value);
                result = year >= 2010 && year <= 2020;
            } catch (...) {
                std::cout << "Couldn't convert " << value << " to number";
            }
        }

        return result;
    }

    bool validateEyr(const std::string& value) {
        bool result = false;

        if (value.size() == 4) {
            try {
                int year = std::stoi(value);
                result = year >= 2020 && year <= 2030;
            } catch (...) {
                std::cout << "Couldn't convert " << value << " to number";
            }
        }

        return result;
    }

    bool validateHgt(const std::string& value) {
        bool result = false;

        if (auto pos = value.find("cm"); pos != std::string::npos) {
            try {
                int height = std::stoi(value.substr(0, pos));
                result = height >= 150 && height <= 193;
            } catch (...) {
                std::cout << "Couldn't convert " << value << " to number";
            }
        } else if (auto pos = value.find("in"); pos != std::string::npos) {
            try {
                int height = std::stoi(value.substr(0, pos));
                result = height >= 59 && height <= 76;
            } catch (...) {
                std::cout << "Couldn't convert " << value << " to number";
            }
        }
        return result;
    }

    bool validateHcl(const std::string& value) {
        const std::regex color_regex("^#[0-9a-fA-F]{6}$");
        return std::regex_match(value, color_regex);
    }

    bool validateEcl(const std::string& value) {
        const static std::vector<std::string> expected_values{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

        return std::any_of(expected_values.cbegin(), expected_values.cend(), [&value](const std::string& expected_value){return expected_value == value;});
    }

    bool validatePid(const std::string& value) {
        const std::regex number_regex("^[0-9]{9}");
        return std::regex_match(value, number_regex);
    }

}

Passport::Passport(std::string passport_string) {
    std::transform(passport_string.begin(), passport_string.end(), passport_string.begin(), [](char a){
        char res = a == '\n' ? ' ' : a;
        return res;});

    auto start = 0;
    auto end = 0;
    while ((start = passport_string.find_first_not_of(" ", end)) != std::string::npos) {
        end = passport_string.find(" ", start);
        auto kv_pair_string = passport_string.substr(start, end - start);
        auto colon_pos = kv_pair_string.find(":");
        auto key = kv_pair_string.substr(0, colon_pos);
        assert(key.size() == 3);
        auto value = kv_pair_string.substr(colon_pos + 1);
        auto [it, success] = fields_.insert({key, value});

        if (!success) throw std::invalid_argument("Couldn't insert " + kv_pair_string);
    }
}

bool Passport::isValid() {
    bool result = false;
    if (auto it = fields_.find("byr"); it != fields_.end()) {
        result = validateByr(it->second);
    }
    if (fields_.find("iyr") == fields_.end()) result = false;
    if (fields_.find("eyr") == fields_.end()) result = false;
    if (fields_.find("hgt") == fields_.end()) result = false;
    if (fields_.find("hcl") == fields_.end()) result = false;
    if (fields_.find("ecl") == fields_.end()) result = false;
    if (fields_.find("pid") == fields_.end()) result = false;

    return result;
}
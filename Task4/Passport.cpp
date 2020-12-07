//
// Created by Adam Kepinski on 07/12/2020.
//

#include "Passport.h"


Passport::Passport(std::string passport_string) {
    std::transform(passport_string.begin(), passport_string.end(), passport_string.begin(), [](char a){
        char res = a == '\n' ? ' ' : a;
        return res;});


    for (auto pos = passport_string.find(" "); pos != std::string::npos; pos = passport_string.find(" ")) {
        auto kv_pair_string = passport_string.substr(0, pos);
        auto colon_pos = kv_pair_string.find(":");
        auto key = kv_pair_string.substr(0, colon_pos);
        assert(key.size() == 3);
        auto value = kv_pair_string.substr(colon_pos + 1);
        auto [it, success] = fields_.insert({key, value});

        if (!success) throw std::invalid_argument("Couldn't insert " + kv_pair_string);

        passport_string.erase(0, pos + 1);
    }
    if (auto colon_pos = passport_string.find(":"); colon_pos != std::string::npos) {
        auto key = passport_string.substr(0, colon_pos);
        assert(key.size() == 3);
        auto value = passport_string.substr(colon_pos + 1);
        auto [it, success] = fields_.insert({key, value});
        if (!success) throw std::invalid_argument("Couldn't insert " + passport_string);
    }
}

bool Passport::isValid() {
    bool result = true;
    if (fields_.find("byr") == fields_.end()) result = false;
    if (fields_.find("iyr") == fields_.end()) result = false;
    if (fields_.find("eyr") == fields_.end()) result = false;
    if (fields_.find("hgt") == fields_.end()) result = false;
    if (fields_.find("hcl") == fields_.end()) result = false;
    if (fields_.find("ecl") == fields_.end()) result = false;
    if (fields_.find("pid") == fields_.end()) result = false;

    return result;
}
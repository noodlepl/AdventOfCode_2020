//
// Created by Adam Kepinski on 07/12/2020.
//

#ifndef ADVENTOFCODE_2020_PASSPORT_H
#define ADVENTOFCODE_2020_PASSPORT_H

#include <string>
#include <map>

class Passport {
    std::map<std::string, std::string> fields_;
public:
    Passport(std::string string_passport);
    bool isValid();
};


#endif //ADVENTOFCODE_2020_PASSPORT_H

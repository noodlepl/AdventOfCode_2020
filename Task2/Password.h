//
// Created by Adam Kepinski on 06/12/2020.
//

#ifndef ADVENTOFCODE_2020_PASSWORD_H
#define ADVENTOFCODE_2020_PASSWORD_H

#import <string>

class Password {
public:
    Password(std::string);

    bool isValid();

private:
    int lower_bound_;
    int upper_bound_;
    char required_character_;
    std::string password_;
};


#endif //ADVENTOFCODE_2020_PASSWORD_H

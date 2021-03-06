//
// Created by Adam Kepinski on 12/12/2020.
//

#ifndef ADVENTOFCODE_2020_POSITIONPART1_H
#define ADVENTOFCODE_2020_POSITIONPART1_H
#include <math.h>

class Position {
    int x_ = 0;
    int y_ = 0;
    int direction_ = 90;

    void moveForward(int value) {
        switch (direction_) {
            case 90:
                x_ += value;
                break;
            case 180:
                y_ -= value;
                break;
            case 270:
                x_ -= value;
                break;
            case 0:
                y_ += value;
                break;
            default:
            {
                std::ostringstream oss;
                oss << "unexpected direction: " << direction_;
                throw std::invalid_argument("unexpected direction");
            }
        }
    }

    void foldDirection() {
        direction_ = fmod(direction_, 360);
        if (direction_ < 0)
            direction_ += 360;
    }

public:
    std::tuple<int, int, int> getValues() {
        return std::make_tuple(x_, y_, direction_);
    }

    int getManhattanDistance() {
        return abs(x_) + abs(y_);
    }

    void move(const std::string& instruction) {
        char type = instruction[0];
        int value = std::stoi(instruction.substr(1));
        switch (type) {
            case 'N':
                y_ += value;
                break;
            case 'S':
                y_ -= value;
                break;
            case 'E':
                x_ += value;
                break;
            case 'W':
                x_ -= value;
                break;
            case 'L':
                direction_ -= value;
                foldDirection();
                break;
            case 'R':
                direction_ += value;
                foldDirection();
                break;
            case 'F':
                moveForward(value);
                break;

        }
    }
};

#endif //ADVENTOFCODE_2020_POSITIONPART1_H

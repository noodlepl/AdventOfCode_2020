//
// Created by Adam Kepinski on 12/12/2020.
//

#ifndef ADVENTOFCODE_2020_POSITIONPART2_H
#define ADVENTOFCODE_2020_POSITIONPART2_H

class Position {
    int x_ = 0;
    int y_ = 0;
    int waypoint_x_ = 10;
    int waypoint_y_ = 1;

    void moveForward(int value) {
        for (int i = 0; i < value; ++i) {
            x_ += waypoint_x_;
            y_ += waypoint_y_;
        }
    }

    void moveWaypointRight(int angle) {
        const int turns_count = angle / 90;
        for (int i = 0; i < turns_count; ++i) {
            std::swap(waypoint_x_, waypoint_y_);
            waypoint_y_ = -waypoint_y_;
        }
    }

    void moveWaypointLeft(int angle) {
        const int turns_count = angle / 90;
        for (int i = 0; i < turns_count; ++i) {
            std::swap(waypoint_x_, waypoint_y_);
            waypoint_x_ = -waypoint_x_;
        }
    }

public:
    std::tuple<int, int, int, int> getValues() {
        return std::make_tuple(x_, y_, waypoint_x_, waypoint_y_);
    }

    int getManhattanDistance() {
        return abs(x_) + abs(y_);
    }

    void move(const std::string &instruction) {
        char type = instruction[0];
        int value = std::stoi(instruction.substr(1));
        switch (type) {
            case 'N':
                waypoint_y_ += value;
                break;
            case 'S':
                waypoint_y_ -= value;
                break;
            case 'E':
                waypoint_x_ += value;
                break;
            case 'W':
                waypoint_x_ -= value;
                break;
            case 'L':
                moveWaypointLeft(value);
                break;
            case 'R':
                moveWaypointRight(value);
                break;
            case 'F':
                moveForward(value);
                break;
        }
    }
};

#endif //ADVENTOFCODE_2020_POSITIONPART2_H

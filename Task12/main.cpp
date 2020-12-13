#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <cstdint>
//#include "PositionPart1.h"
#include "PositionPart2.h"

int calculateDistance() {
    Position position;
    std::ifstream ifs("input.txt");
    std::string instruction;
    while(std::getline(ifs, instruction)) {
        position.move(instruction);
    }

//    auto [x, y, direction] = position.getValues();
//    std::cout << "x: " << x << ", y: " << y << ", direction: " << direction << "\n";

    auto [x, y, waypoint_x, waypoint_y] = position.getValues();
    std::cout << "x: " << x << ", y: " << y << ", waypoint_x: " << waypoint_x << ", waypoint_y: " << waypoint_y << "\n";

    return position.getManhattanDistance();
}

int main() {
    auto start = std::chrono::system_clock::now();

    std::cout << "Distance from start " << calculateDistance() << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
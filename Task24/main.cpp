#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <unordered_set>
#include <sstream>

struct Coordinate {
    int x = 0;
    int y = 0;

    void moveE()  {x += 1;}
    void moveW()  {x -= 1;}
    void moveNw() {y += 1;}
    void moveNe() {x += 1;y += 1;}
    void moveSe() {y -= 1;}
    void moveSw() {x -= 1;y -= 1;}
    void move(const std::string& instruction) {
        if      (instruction == "w") moveW();
        else if (instruction == "e") moveE();
        else if (instruction == "nw") moveNw();
        else if (instruction == "ne") moveNe();
        else if (instruction == "se") moveSe();
        else if (instruction == "sw") moveSw();
    }
    friend bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

struct CoordHash
{
    std::size_t operator()(Coordinate const& coordinate) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(coordinate.x);
        std::size_t h2 = std::hash<int>{}(coordinate.y);
        return h1 ^ (h2 << 1);
    }
};

using Tiles = std::unordered_set<Coordinate, CoordHash>;
int max_x = 0;
int max_y = 0;
int min_x = 0;
int min_y = 0;

void flipTile(Coordinate tile, Tiles& black_tiles) {
    if (black_tiles.find(tile) == black_tiles.end()) {
        black_tiles.insert(tile);
        max_x = std::max(max_x, tile.x);
        max_y = std::max(max_y, tile.y);
        min_x = std::min(min_x, tile.x);
        min_y = std::min(min_y, tile.y);
    }
    else {
        black_tiles.erase(tile);
    }
}

void flipTile(std::string instructions, Tiles& black_tiles) {
    std::istringstream iss(instructions);
    char c;
    Coordinate target_coord;
    while(iss >> c) {
        std::ostringstream instruction_ss;
        switch (c) {
            case 's':
            case 'n':
                instruction_ss << c;
                iss >> c;
                assert(c == 'e' || c == 'w');
                instruction_ss << c;
                break;
            case 'e':
            case 'w':
                instruction_ss << c;
                break;
        }
        auto instruction = instruction_ss.str();
        target_coord.move(instruction);
    }
    flipTile(std::move(target_coord), black_tiles);
}

bool checkSurrounding(const Coordinate& coord, const Tiles& black_tiles) {
    int surrounding_blacks = 0;
    if (black_tiles.find(Coordinate{coord.x-1, coord.y-1}) != black_tiles.end()) ++surrounding_blacks;
    if (black_tiles.find(Coordinate{coord.x-1, coord.y}) != black_tiles.end()) ++surrounding_blacks;
    if (black_tiles.find(Coordinate{coord.x, coord.y-1}) != black_tiles.end()) ++surrounding_blacks;
    if (black_tiles.find(Coordinate{coord.x, coord.y+1}) != black_tiles.end()) ++surrounding_blacks;
    if (black_tiles.find(Coordinate{coord.x+1, coord.y}) != black_tiles.end()) ++surrounding_blacks;
    if (black_tiles.find(Coordinate{coord.x+1, coord.y+1}) != black_tiles.end()) ++surrounding_blacks;

    bool is_black = black_tiles.find(coord) != black_tiles.end();
    if (is_black && (surrounding_blacks == 0 || surrounding_blacks > 2)) return true;
    if (!is_black && surrounding_blacks == 2) return true;
    return false;
}

int main() {
    auto start = std::chrono::system_clock::now();

    std::ifstream ifs("input.txt");
    std::string line;
    Tiles black_tiles;

    while(std::getline(ifs, line)) {
        flipTile(std::move(line), black_tiles);
    }

    std::cout << "Result: " << black_tiles.size() << " black tiles remaining\n";
    std::cout << "Border coords (min_x, min_y, max_x, max_y): " << min_x << ", " << min_y << ", " << max_x << ", " << max_y << "\n";

    int days = 0;
    while (days++ != 100) {
        Tiles next_gen_black_tiles = black_tiles;
        for (int x = min_x-1; x <= max_x+1;++x){
            for (int y = min_y-1; y <= max_y+1;++y){
                Coordinate coord{x, y};
                if (checkSurrounding(coord, black_tiles))
                    flipTile(coord, next_gen_black_tiles);
            }
        }
        black_tiles = next_gen_black_tiles;
        std::cout << "Day " << days << ": " << black_tiles.size() << " black tiles\n";
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
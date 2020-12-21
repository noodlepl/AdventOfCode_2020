#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <sstream>
#include <cstdint>
#include <map>
#include <regex>
#include <array>
#include <list>

using Image = std::array<std::array<char, 10>, 10>;
using EdgesIds = std::map<int64_t, std::array<std::pair<std::bitset<10>, std::bitset<10>>, 4>>;
using ImagesMap = std::map<int64_t, Image>;

std::pair<std::bitset<10>,std::bitset<10>> getEdgeId(std::array<char, 10> edge) {
    auto f = std::bitset<10>();
    auto r = std::bitset<10>();
    auto itf = edge.begin();
    auto itr = edge.rbegin();
    auto mask = std::bitset<10>(1);
    for (;itf != edge.end(); ++itf, ++itr, mask <<= 1) {
        if (*itf == '#') f |= mask;
        if (*itr == '#') r |= mask;
    }

    return {f, r};
}

std::pair<ImagesMap, EdgesIds> readData() {
    std::ifstream ifs("input.txt");
    std::string line;
    std::map<int64_t, Image> images;
    EdgesIds edges;
    int row_id = 0;
    while (std::getline(ifs, line)) {
        int64_t key;
        if (line.empty()) {
            row_id = 0;
        } else if (auto it = std::find_if(line.begin(), line.end(), isdigit); it != line.end()) {
            key = std::stoi(line.substr(it - line.begin(), 4));
            auto [insert_it_img, success_img] = images.insert({key, Image()});
            auto [insert_it_edg, success_edg] = edges.insert({key, std::array<std::pair<std::bitset<10>, std::bitset<10>>, 4>()});
            assert(success_img);
            assert(success_edg);
        } else {
            std::copy(line.begin(), line.end(), images[key][row_id++].begin());

            // top_id
            {
                auto edge = images[key].front();
                auto id = getEdgeId(edge);
                edges[key][0] = id;
            }

            // right_id
            {
                auto edge = std::array<char, 10>();
                std::transform(images[key].begin(), images[key].end(), edge.begin(), [](auto row) {
                   return row.back();
                });
                auto id = getEdgeId(edge);
                edges[key][1] = id;
            }

            // bottom_id
            {
                auto edge = images[key].back();
                auto id = getEdgeId(edge);
                edges[key][2] = id;
            }

            // left_id
            {
                auto edge = std::array<char, 10>();
                std::transform(images[key].begin(), images[key].end(), edge.begin(), [](auto row) {
                    return row.front();
                });
                auto id = getEdgeId(edge);
                edges[key][3] = id;
            }
        }
    }

    return {images, edges};
}

template <typename T>
T& flip(T& image) {
    for (auto&& row : image) {
        std::reverse(row.begin(), row.end());
    }

    return image;
}

template <typename T>
T& rotate(T& image) {
    auto size = image.size();
    assert(size == image[0].size());

    for (int i = 0; i < size / 2; ++i) {
        for (int j = i; j < size - i - 1; ++j) {
            typename T::value_type::value_type temp = image[i][j];
            image[i][j] = image[size - 1 - j][i];
            image[size - 1 - j][i] = image[size - 1 - i][size - 1 - j];
            image[size - 1 - i][size - 1 - j] = image[j][size - 1 - i];
            image[j][size - 1 - i] = temp;
        }
    }

    return image;
}

struct Tile {
    int64_t key = -1;
    Image image;
};

// side: top = 0, right = 1, bottom = 2, left = 3
std::vector<char> getEdge(const Image& image, int side, bool reversed = false) {
    std::vector<char> edge;
    edge.reserve(image.size());
    switch (side) {
        case 0:
            std::copy(image[0].begin(), image[0].end(), std::back_inserter(edge));
            break;
        case 1:
            std::transform(image.begin(), image.end(), std::back_inserter(edge), [](auto&& row) {return row.back();});
            break;
        case 2:
            std::copy(image.back().begin(), image.back().end(), std::back_inserter((edge)));
            break;
        case 3:
            std::transform(image.begin(), image.end(), std::back_inserter(edge), [](auto&& row) {return row[0];});
            break;
    }
    if (reversed)
        std::reverse(edge.begin(), edge.end());
    return edge;
}

auto handleTopLeftCorner(std::list<Tile>& tiles, int64_t top_left_key) {
    auto detailed_top_left_it = std::find_if(tiles.begin(), tiles.end(), [top_left_key](auto&& image) {
        return image.key == top_left_key;
    });
    bool found = false;
    for (int i = 0; i < 4; ++i) {
        auto matches_right = std::find_if(tiles.begin(), tiles.end(), [detailed_top_left_it](auto&& image) {
           if (image.key == detailed_top_left_it->key)
               return false;
           auto right_edge = getEdge(detailed_top_left_it->image, 1);
           for (int tested_i = 0; tested_i < 4; ++tested_i) {
               if (right_edge == getEdge(image.image,tested_i) || right_edge == getEdge(image.image,tested_i, true))
                   return true;
           }
           return false;
        });
        if (matches_right != tiles.end()){
            auto matches_bottom = std::find_if(tiles.begin(), tiles.end(), [detailed_top_left_it](auto&& image) {
                if (image.key == detailed_top_left_it->key)
                    return false;
                auto bottom_edge = getEdge(detailed_top_left_it->image, 2);
                for (int tested_i = 0; tested_i < 4; ++tested_i) {
                    if (bottom_edge == getEdge(image.image,tested_i) || bottom_edge == getEdge(image.image,tested_i, true))
                        return true;
                }
                return false;
            });
            if (matches_bottom != tiles.end()) {
                found = true;
                break;
            }
        }
        rotate(detailed_top_left_it->image);
    }
    assert (found);
    return detailed_top_left_it;
}

bool matchTile(Image& image, const std::vector<char>& reference_edge, int edge_to_fit) {
    for (int rotation = 0; rotation < 4; ++rotation) {
        auto left_edge = getEdge(image, edge_to_fit);
        if (reference_edge == left_edge)
            return true;

        flip(image);
        left_edge = getEdge(image, edge_to_fit);
        if (reference_edge == left_edge)
            return true;

        flip(image);
        rotate(image);
    }
    return false;
}

void printTile(const Tile& tile) {
//    return;
    std::cout << tile.key << ":\n";
    for (auto&& row : tile.image) {
        for (auto c : row)
            std::cout << c;
        std::cout << "\n";
    }
    std::cout << "\n";
}

template<typename Img>
bool markMonsters(Img& image) {
    bool changed = false;

    std::array<std::pair<int, int>, 15> monster_ids = {{{1,0}, {2,1}, {2,4}, {1,5}, {1,6},
                                                        {2,7}, {2,10},{1,11},{1,12},{2,13},
                                                        {2,16}, {1,17}, {0,18}, {1,18},{1,19}}};

    for (int row = 0; row < image.size() - 2; ++row) {
        for (int col = 0; col < image.size() - 19; ++col) {
            bool monster = true;
            for (auto&& id : monster_ids) {
                int peeked_row = row + id.first;
                int peeked_col = col + id.second;
                if (image[peeked_row][peeked_col] != '#') {
                    monster = false;
                    break;
                }
            }
            if (monster) {
                changed = true;
                for (auto&& id : monster_ids) {
                    image[row + id.first][col + id.second] = 'O';
                }
            }
        }
    }
    return changed;
}

int main() {
    auto start = std::chrono::system_clock::now();

    auto [images, edges_ids] = readData();

//    for (auto&& kv : images) {
//        std::cout << kv.first << ":\n";
//        for (auto&& row : kv.second) {
//            for (auto elem : row) {
//                std::cout << elem;
//            }
//            std::cout << "\n";
//        }
//        std::cout << "Top id: " << edges_ids[kv.first][0].to_string()<< "\n";
//        std::cout << "Right id: " << edges_ids[kv.first][1].to_string()<< "\n";
//        std::cout << "Bottom id: " << edges_ids[kv.first][2].to_string()<< "\n";
//        std::cout << "Left id: " << edges_ids[kv.first][3].to_string()<< "\n";
//        std::cout << "\n";
//    }

    std::map<int64_t, int> matching_edges;
    using UniqueEdgeIds = std::map<int64_t, std::array<std::bitset<10>, 4>>;
    UniqueEdgeIds unique_edge_ids;
    for (auto&& kv : edges_ids) {
        UniqueEdgeIds::mapped_type edge_array;
        std::transform(kv.second.begin(), kv.second.end(), edge_array.begin(), [](const auto& edge) {
            return std::min(edge.first, edge.second, [](auto forward, auto reverse) {
                return forward.to_ulong() < reverse.to_ulong();
            });
        });
        unique_edge_ids[kv.first] = edge_array;
    }

    for (auto&& kv : unique_edge_ids) {
        auto key = kv.first;
        auto& searched_edges = kv.second;
        matching_edges[key] = 0;
        for (auto&& edge : searched_edges) {
            auto edge_count = std::count_if(unique_edge_ids.begin(), unique_edge_ids.end(), [&edge](auto edge_ids) {
                for (auto&& id : edge_ids.second) {
                    if (id == edge)
                        return true;
                }
                return false;
            });
            if (edge_count == 2) {
                ++matching_edges[key];
            }
        }
    }

    std::vector<std::map<int64_t, int>::value_type> corner_candidates;
    std::copy_if(matching_edges.begin(), matching_edges.end(), std::back_inserter(corner_candidates), [](auto&& kv){
        return kv.second == 2;
    });
    std::cout << "corners are: ";
    for (auto&& candidate : corner_candidates) {
        std::cout << candidate.first << ",";
    }
    std::cout << std::endl;
    int64_t result = std::accumulate(corner_candidates.begin(), corner_candidates.end(), 1LL, [](int64_t current, auto&& next) {
        return current * next.first;
    });

    std::cout << "Result: " << result << "\n";

    using TilesMatrix = std::vector<std::vector<Tile>>;
    int dimension = sqrt(images.size());
    TilesMatrix tiles_mat(dimension, std::vector<Tile>(dimension, Tile()));
    std::list<Tile> tiles_list;

    for (auto&& kv : images) {
        auto key = kv.first;
        auto& image = kv.second;
        auto& edges = edges_ids[key];
        tiles_list.push_back(Tile{key, image});
    }

    auto tl_tile = handleTopLeftCorner(tiles_list, corner_candidates.begin()->first);

    tiles_mat[0][0] = *tl_tile;
    tiles_list.erase(tl_tile);

    printTile(tiles_mat[0][0]);
    for (auto row_it = tiles_mat.begin(); row_it != tiles_mat.end(); ++row_it) {
        if (row_it->begin()->key == -1) {
            auto on_top_it = std::prev(row_it)->begin();
            auto reference_edge = getEdge(on_top_it->image, 2);
            for (auto tile_it = tiles_list.begin(); tile_it != tiles_list.end();++tile_it) {
                if (matchTile(tile_it->image, reference_edge, 0)) {
                    *(row_it->begin()) = *tile_it;
                    printTile(*tile_it);
                    tiles_list.erase(tile_it);
                    break;
                }
            }
        }
        for (auto col_it = std::next(row_it->begin()); col_it != row_it->end(); ++col_it) {
            auto on_left_it = std::prev(col_it);
            auto reference_edge = getEdge(on_left_it->image, 1);
            for (auto tile_it = tiles_list.begin(); tile_it != tiles_list.end();++tile_it) {
                if (matchTile(tile_it->image, reference_edge, 3)) {
                    *col_it = *tile_it;
                    printTile(*tile_it);
                    tiles_list.erase(tile_it);
                    break;
                }
            }
        }
    }

    result = tiles_mat[0][0].key * tiles_mat[0].back().key * tiles_mat.back()[0].key * tiles_mat.back().back().key;

    using BigImage = std::vector<std::vector<char>>;
    BigImage big_image(tiles_mat.size() * 8, std::vector<char>(tiles_mat[0].size() * 8));
    for (int row = 0; row < tiles_mat.size(); row++) {
        for (int col = 0; col < tiles_mat[0].size(); col++) {
            for (int img_row = 1; img_row < 9; img_row++) {
                auto& data = tiles_mat[row][col].image[img_row];
                auto row_id = row*8+img_row;
                std::copy(std::next(data.begin()), std::prev(data.end()), big_image[row_id - 1].begin() + col*8);
            }
        }
    }

    for (auto&& row : big_image) {
        for (auto c : row)
            std::cout << c;
        std::cout << "\n";
    }
    std::cout << "\n";

    int count = 0;
    while(!markMonsters(big_image)) {
        if (count++ % 2)
            rotate(big_image);
        else
            flip(big_image);

        for (auto&& row : big_image) {
            for (auto c : row)
                std::cout << c;
            std::cout << "\n";
        }
        std::cout << "\n";
        if (count >= 8) {
            std::cout << "Failure!\n";
            break;
        }
    }

    int64_t remaining_water = std::accumulate(big_image.begin(), big_image.end(), 0LL, [](auto sum, auto&& row) {
        int64_t count_row = std::count(row.begin(), row.end(), '#');
        return sum + count_row;
    });

    std::cout << "Part 2 result: " << remaining_water << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
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
using EdgesIds = std::map<int64_t, std::array<std::bitset<10>, 4>>;
using ImagesMap = std::map<int64_t, Image>;

std::bitset<10> getEdgeId(std::array<char, 10> edge) {
    auto f = std::bitset<10>();
    auto r = std::bitset<10>();
    auto itf = edge.begin();
    auto itr = edge.rbegin();
    auto mask = std::bitset<10>(1);
    for (;itf != edge.end(); ++itf, ++itr, mask <<= 1) {
        if (*itf == '#') f |= mask;
        if (*itr == '#') r |= mask;
    }

    return std::min(f, r, [](auto a, auto b) {
        return a.to_ulong() < b.to_ulong();
    });
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
            auto [insert_it_edg, success_edg] = edges.insert({key, std::array<std::bitset<10>, 4>()});
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

    for (auto&& kv : edges_ids) {
        auto key = kv.first;
        auto& searched_edges = kv.second;
        matching_edges[key] = 0;
        for (auto&& edge : searched_edges) {
            auto edge_count = std::count_if(edges_ids.begin(), edges_ids.end(), [&edge](auto edge_ids) {
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

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
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
#include <unordered_map>
#include <unordered_set>

std::pair<std::vector<std::string>, std::list<std::string>> parseLine(std::string line) {
    std::vector<std::string> keys;
    std::list<std::string> ingredients;
    const std::string delimiter = "(contains ";
    auto pos = line.find(delimiter);
    auto ingredients_str = line.substr(0, pos);
    std::istringstream ingredients_iss(ingredients_str);
    std::string ingredient;
    while (ingredients_iss >> ingredient)
        ingredients.push_back(ingredient);

    auto keys_len = line.size() - pos - delimiter.size() - 1;
    auto keys_str = line.substr(pos + delimiter.size(), keys_len);
    keys_str.erase(std::remove(keys_str.begin(), keys_str.end(), ' '), keys_str.end());
    std::istringstream keys_iss(keys_str);
    std::string key;
    while (std::getline(keys_iss, key, ','))
        keys.push_back(key);

    return {keys, ingredients};
}

int main() {
    auto start = std::chrono::system_clock::now();
    using AllergensProductsMap = std::map<std::string, std::vector<std::list<std::string>>>;
    using CantContain = std::vector<std::string>;
    using CheckedIngredients = std::unordered_set<std::string>;

    std::ifstream ifs("input.txt");
    std::string line;

    // read input
    AllergensProductsMap might_contain;
    std::vector<std::list<std::string>> all_products;
    while (std::getline(ifs, line)) {
        auto [allergen_keys, ingredients] = parseLine(std::move(line));

        for (auto&& key : allergen_keys) {
            might_contain[key].push_back(ingredients);
        }
        all_products.push_back(ingredients);
    }

    CantContain cant_contain_any;
    CheckedIngredients checked_ingredients;
    for(auto&& allergen_products : might_contain) {
        auto& products = allergen_products.second;
        for (auto&& product : products) {
            for (auto&& ingredient : product) {
                if (std::find(checked_ingredients.begin(), checked_ingredients.end(), ingredient) != checked_ingredients.end())
                    continue;
                if (std::all_of(might_contain.begin(), might_contain.end(), [ingredient](auto&& allergen_products){
                    auto& products = allergen_products.second;
                    return std::any_of(products.begin(), products.end(), [ingredient](auto&& product) {
                        return std::find(product.begin(), product.end(), ingredient) == product.end();
                    });
                })) {
                    cant_contain_any.push_back(ingredient);
                    checked_ingredients.insert(ingredient);
                }
            }
        }
    }

    for (auto&& ingredient : cant_contain_any) {
        std::cout << ingredient << ", ";
    }
    std::cout << "\n";

    auto result = std::accumulate(all_products.begin(), all_products.end(), 0, [&cant_contain_any](int sum, auto&& product) {
       int safe_ingredients_count = 0;
       sum += std::count_if(product.begin(), product.end(), [&cant_contain_any](auto&& ingredient) {
           return std::find(cant_contain_any.begin(), cant_contain_any.end(), ingredient) != cant_contain_any.end();
       });
       return sum + safe_ingredients_count;
    });

    std::cout << "Part 1 result: " << result << "\n";

    // Part 2
    // Remove safe ingredients
    std::map<std::string, std::unordered_set<std::string>> allergen_candidates;
    for (auto&& allergen_products : might_contain) {
        auto key = allergen_products.first;
        auto& products = allergen_products.second;
        for (auto&& product : products) {
            for (auto it = product.begin(); it != product.end();) {
                if (std::find(cant_contain_any.begin(), cant_contain_any.end(), *it) != cant_contain_any.end())
                    it = product.erase(it);
                else
                    allergen_candidates[key].insert(*it++);
            }
        }
    }
    //print
//    for (auto&& allergen_products : might_contain) {
//        std::cout << allergen_products.first << ": ";
//        for (auto&& product : allergen_products.second) {
//            for (auto&& ingredient : product) {
//                std::cout << ingredient << ", ";
//            }
//            std::cout << "\n";
//        }
//        std::cout << "\n";
//    }

    int count = 0;
    while (std::any_of(allergen_candidates.begin(), allergen_candidates.end(), [](auto&& kv){return kv.second.size() != 1;})) {
        for (auto&& kv : allergen_candidates) {
            auto& products_with_candidate = might_contain[kv.first];
            auto& candidates = kv.second;
            if (candidates.size() == 1) {
                for (auto&& inner_kv : allergen_candidates) {
                    if (inner_kv.first == kv.first) continue;
                    auto& other_candidates = inner_kv.second;
                    other_candidates.erase(*candidates.begin());
                }
            }
            for (auto it = candidates.begin(); it != candidates.end();) {
                auto& candidate = *it;
                if (std::any_of(products_with_candidate.begin(), products_with_candidate.end(), [candidate](auto&& product) {
                    return std::find(product.begin(), product.end(), candidate) == product.end();
                })) {
                    it = candidates.erase(it);
                    continue;
                }
                ++it;
            }
        }
        std::cout << "iteration: " << count++ << "\n";
    }

    //print
    for (auto&& kv : allergen_candidates) {
        std::cout << kv.first << ": ";
        for (auto&& candidate : kv.second)
            std::cout << candidate << ", ";
        std::cout << "\n";
    }

    std::ostringstream cdil;
    for (auto&& kv : allergen_candidates) {
        if (kv != *allergen_candidates.begin())
            cdil << ",";
        cdil << *kv.second.begin();
    }

    std::cout << "Part 2 canonical dangerous ingredient list: " << cdil.str() << "\n";

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
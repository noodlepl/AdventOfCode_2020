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

std::pair<std::vector<std::string>, std::vector<std::string>> parseLine(std::string line) {
    std::vector<std::string> keys;
    std::vector<std::string> ingredients;
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
    using AllergensProductsMap = std::map<std::string, std::vector<std::vector<std::string>>>;
    using CantContain = std::vector<std::string>;
    using CheckedIngredients = std::unordered_set<std::string>;

    std::ifstream ifs("input.txt");
    std::string line;

    // read input
    AllergensProductsMap might_contain;
    std::vector<std::vector<std::string>> all_products;
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

    auto end = std::chrono::system_clock::now();
    std::chrono::microseconds diff = end - start;
    std::cout << "Program duration: " << diff.count() << " microseconds" << std::endl;
    return 0;
}
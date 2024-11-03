#include "..\h\intrinsic_functions.h"

namespace intrinsic_functions {

    
    bool custom_comparator(const number& a, const number& b) {
        return std::visit([](const auto& first, const auto& scnd) {
            return first < scnd;
            }, a, b);
    }


    number max(const std::vector<number>& nums) {
        return *std::max_element(nums.begin(), nums.end(), custom_comparator);
    }


    number min(const std::vector<number>& nums) {
        return *std::min_element(nums.begin(), nums.end(), custom_comparator);
    }


    number avg(const std::vector<number>& nums) {
        return std::accumulate(nums.begin(), nums.end(), 0.0, [](double acc, const number& var) {
            return acc + std::visit([](const auto& value) -> double {
                return static_cast<double>(value);
                }, var);
            }) / nums.size();
    }


    number sum(const std::vector<number>& nums) {
        return std::accumulate(nums.begin(), nums.end(), 0.0, [](double acc, const number& var) {
            return acc + std::visit([](const auto& value) -> double {
                return static_cast<double>(value);
                }, var);
            });
    }
}

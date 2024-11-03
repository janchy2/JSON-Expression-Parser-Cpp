#pragma once
#include <unordered_map>
#include <functional>
#include <variant>
#include <algorithm>
#include <numeric>

namespace intrinsic_functions {

	// these functions can only operate on numbers
	using number = std::variant<int, double>;

	// every intrinsic function that operates on an array
	number max(const std::vector<number>& nums);
	number min(const std::vector<number>& nums);
	number avg(const std::vector<number>& nums);
	number sum(const std::vector<number>& nums);

}
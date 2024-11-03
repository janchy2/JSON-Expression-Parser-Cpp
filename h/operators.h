#pragma once
#include <stack>

#include "shared_structures.h"

namespace operators {


	using number = std::variant<int, double>;

	number calculate_operators(const std::vector<number>& operands, const std::vector<char>& operators);

}
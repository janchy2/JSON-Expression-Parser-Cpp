#pragma once
#include <vector>

#include "shared_structures.h"

namespace parser {

	// functions that take vectors of tokens and parse them

	jsonValue parse_json(const std::vector<jsonToken>& tokens);

	jsonValue parse_and_evaluate_expression(const std::vector<expressionToken>& tokens, jsonValue& startElement);

}

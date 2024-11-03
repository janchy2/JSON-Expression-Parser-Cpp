#pragma once
#include "shared_structures.h"
#include "espression_type_checks.h"
#include "json_type_checks.h"


namespace tokenizer {

	// functions for extracting tokens

	std::vector<jsonToken> tokenize_json(const std::string& jsonString);

	std::vector<expressionToken> tokenize_expression(const std::string& expression);

}

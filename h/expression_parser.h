#pragma once
#include "shared_structures.h"

namespace expression_parser {


	jsonValue parse_and_eval_expression_element(jsonValue& jsonElement, const std::vector<expressionToken>& tokens, int& position, jsonValue& startElem);

}
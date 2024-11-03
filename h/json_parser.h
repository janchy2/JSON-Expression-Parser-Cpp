#pragma once
#include "shared_structures.h"

namespace json_parser {


	jsonValue parse_json_element(const std::vector<jsonToken>& tokens, int& position);

}
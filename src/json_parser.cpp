#include "..\h\json_parser.h"

namespace json_parser {


	jsonValue parse_json_value(const std::vector<jsonToken>& tokens, int& position) {
		return std::get<valueToken>(tokens[position++]);
	}


	jsonValue parse_json_array(const std::vector<jsonToken>& tokens, int& position) {
		jsonArray arr;
		if (tokens[position].index() == 3) {
			// right bracket, end
			++position;
			return arr;
		}
		while (true) {
			arr.emplace_back(parse_json_element(tokens, position));
			if (tokens[position].index() == 3) {
				// right bracket, end
				++position;
				break;
			}
			// next token must be a comma (throws an exception if not)
			std::get<comma>(tokens[position]);
			++position;
		}
		return arr;
	}


	jsonValue parse_json_object(const std::vector<jsonToken>& tokens, int& position) {
		jsonObject map;
		if (tokens[position].index() == 1) {
			// right brace, end
			++position;
			return map;
		}
		while (true) {
			// token must be string (key) (throws an exception if not)
			std::string key = std::get<std::string>(std::get<valueToken>(tokens[position]));
			//next token must be a colon (throws an exception if not)
			std::get<colon>(tokens[++position]);
			map[key] = jsonValueWrapper(parse_json_element(tokens, ++position));
			if (tokens[position].index() == 1) {
				// right brace, end
				++position;
				break;
			}
			// next token must be a comma (throws an exception if not)
			std::get<comma>(tokens[position]);
			++position;
		}
		return map;
	}


	jsonValue parse_json_element(const std::vector<jsonToken>& tokens, int& position) {
		return std::visit(overloaded {
				[&](const valueToken&) -> jsonValue {
					// type is null, bool, string or number (value token)
					return parse_json_value(tokens, position);
				},
				[&](const left_bracket&) -> jsonValue {
					// this is an array
					return parse_json_array(tokens, ++position);
				},
				[&](const left_brace&) -> jsonValue {
					// this is an object
					return parse_json_object(tokens, ++position);
				},
				[](const right_brace&) -> jsonValue { throw std::runtime_error("Invalid JSON!"); },
				[](const right_bracket&) -> jsonValue { throw std::runtime_error("Invalid JSON!"); },
				[](const comma&) -> jsonValue { throw std::runtime_error("Invalid JSON!"); },
				[](const colon&) -> jsonValue { throw std::runtime_error("Invalid JSON!"); }
				}, tokens[position]);
	}
}

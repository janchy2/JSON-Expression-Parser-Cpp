#include "..\h\parser.h"
#include "..\h\json_parser.h"
#include "..\h\expression_parser.h"

namespace parser {


	jsonValue parse_json(const std::vector<jsonToken>& tokens) {
		int position = 0;
		try {
			jsonValue result = json_parser::parse_json_element(tokens, position);
			if (position != tokens.size()) {
				throw std::runtime_error("Invalid JSON!");
			}
			return result;
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Invalid JSON!");
		}
	}

	jsonValue parse_and_evaluate_expression(const std::vector<expressionToken>& tokens, jsonValue& startElement) {
		int position = 0;
		try {
			jsonValue result = expression_parser::parse_and_eval_expression_element(startElement, tokens, position, startElement);
			if (position != tokens.size()) {
				throw std::runtime_error("Invalid expression!");
			}
			return result;
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Invalid expression!");
		}
	}

}

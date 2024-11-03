#include "..\h\tokenizer.h"

namespace tokenizer {


	std::vector<jsonToken> tokenize_json(const std::string& jsonString) {
		std::vector<jsonToken> jsonTokens;
		int position = 0;
		while (position != jsonString.size()) {
			// iterate through json
			try {
				if (json_tokenizer::typeMap.contains(jsonString[position])) {
					// check if character is easily recognizes as type signifier
					json_tokenizer::typeMap[jsonString[position]](jsonString, position, jsonTokens);
				}
				else {
					// number is the only type that we cannot check through the unordered map (it has many options for first char)
					json_tokenizer::check_number_and_add(jsonString, position, jsonTokens);
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Invalid JSON!");
			}
		}
		return jsonTokens;
	}


	std::vector<expressionToken> tokenize_expression(const std::string& expression) {
		std::vector<expressionToken> expressionTokens;
		int position = 0;
		while (position != expression.size()) {
			// iterate through expression
			try {
				if (expression_tokenizer::typeMap.contains(expression[position])) {
					// check if character is easily recognizes as type signifier
					expression_tokenizer::typeMap[expression[position]](expression, position, expressionTokens);
				}
				// number, string and functions are types that we cannot check through the unordered map (they have many options for first char)
				else if (expression_tokenizer::check_number_and_add(expression, position, expressionTokens)) {
					continue;
				}
				else if (expression_tokenizer::check_string_or_func_and_add(expression, position, expressionTokens)) {
					continue;
				}
				else {
					throw std::runtime_error("Invalid expression!");
				}
			}
			catch (const std::exception& e) {
				throw std::runtime_error("Invalid expression!");
			}
		}
		return expressionTokens;
	}
}

#include "..\h\expression_parser.h"

namespace expression_parser {


	jsonValue parse_and_eval_expression_key(jsonValue& jsonElement, const std::vector<expressionToken>& tokens, int& position, jsonValue& startElement) {
		// the element should be an object (throws exception otherwise), get the value for key and continue evaluation for it
		jsonValue nextValue = std::get<jsonObject>(jsonElement)[std::get<std::string>(tokens[position])].value;
		return parse_and_eval_expression_element(nextValue, tokens, ++position, startElement);
	}


	jsonValue parse_and_eval_expression_subscript(jsonValue& jsonElement, const std::vector<expressionToken>& tokens, int& position, jsonValue& startElement) {
		// evaluate expression in brackets and the result should be an int (index) (throws exception otherwise)
		jsonValue indexJsonValue = parse_and_eval_expression_element(startElement, tokens, ++position, startElement);
		int index = std::get<int>(std::get<valueToken>(indexJsonValue));
		// next token should be right bracket (throws exception otherwise)
		std::get<right_bracket>(tokens[position]);
		// the element should be an array (throws exception otherwise), get the element of vector and continue evaluation for it
		return parse_and_eval_expression_element(std::get<jsonArray>(jsonElement)[index].value, tokens, ++position, startElement);
	}

	void convert_json_to_number_array(jsonArray& arr, std::vector<intrinsic_functions::number>& results) {
		for (jsonValueWrapper& element : arr) {
			// it has to be either a double or an int (otherwise throws an exception)
			valueToken value = std::get<valueToken>(element.value);
			if (value.index() == 2) {
				results.emplace_back(std::get<double>(value));
			}
			else {
				results.emplace_back(std::get<int>(value));
			}
		}
	}


	jsonValue parse_and_eval_expression_func(jsonValue& jsonElement, const std::vector<expressionToken>& tokens, int& position, jsonValue& startElement) {
		intrinsicFunc iFunc = std::get<intrinsicFunc>(tokens[position]);
		// next token must be left parentheses (throws exception otherwise)
		std::get<left_par>(tokens[++position]);
		std::vector<intrinsic_functions::number> results;
		while (true) {
			jsonValue argument = parse_and_eval_expression_element(startElement, tokens, ++position, startElement);
			if (!argument.index()) {
				// only one argument, an array
				jsonArray arr = std::get<jsonArray>(argument);
				convert_json_to_number_array(arr, results);
				++position;
				break;
			}
			// multiple arguments
			valueToken result = std::get<valueToken>(argument);
			if (result.index() == 2) {
				// double
				results.emplace_back(std::get<double>(result));
			}
			else {
				// int or something invalid (throws exception)
				results.emplace_back(std::get<int>(result));
			}
			if (tokens[position].index() == 8) {
				// right paretheses
				++position;
				break;
			}
			// next token must be comma (throws exception otherwise)
			std::get<comma>(tokens[position]);
		}
		// call intrinsic function for results
		intrinsic_functions::number functionResult =  iFunc(results);
		return std::visit(overloaded{
			[](int& i) -> jsonValue {
				return i;
			},
			[](double& d) -> jsonValue {
				return d;
			}
			}, functionResult);
	}


	jsonValue parse_and_eval_expression_size(jsonValue& jsonElement, const std::vector<expressionToken>& tokens, int& position, jsonValue& startElement) {
		// next token must be left parentheses (throws exception otherwise)
		std::get<left_par>(tokens[++position]);
		jsonValue result = parse_and_eval_expression_element(startElement, tokens, ++position, startElement);
		jsonValue size = std::visit(overloaded{
				// result must be an object, array or string (throws exception otherwise)
				[&](jsonObject& obj) -> jsonValue {
					return static_cast<int>(obj.size());
				},
				[&](jsonArray& arr) -> jsonValue {
					return static_cast<int>(arr.size());
				},
				[&](valueToken& val) -> jsonValue {
					return static_cast<int>(std::get<std::string>(val).size());
				}
				}, result);
		// next token must be right parentheses (throws exception otherwise)
		std::get<right_par>(tokens[position]);
		++position;
		return size;
	}


	jsonValue parse_and_eval_expression_element(jsonValue& jsonElement, const std::vector<expressionToken>& tokens, int& position, jsonValue& startElement) {
		if (position == tokens.size() || jsonElement.index() == 2) {
			// end of expression or element is not an object or an array, we cannot go further
			return jsonElement;
		}
			
		return std::visit(overloaded {
				// number literals, to be valid, jsonElement must be == startElement
				[&](const int& i) -> jsonValue {
					if (&jsonElement != &startElement) {
						throw std::runtime_error("Invalid expression!");
					}
					++position;
					return i;
				},
				[&](const double& d) -> jsonValue {
					if (&jsonElement != &startElement) {
						throw std::runtime_error("Invalid expression!");
					}
					++position;
					return d;
				},
				[&](const std::string& s) -> jsonValue {
					// this is a key, it needs to be one of the root keys to be valid (jsonElem == startElem)
					if (&jsonElement == &startElement) {
						return parse_and_eval_expression_key(jsonElement, tokens, position, startElement);
					}
					else {
						throw std::runtime_error("Invalid expression!");
					}
				},
				[&](const dot&) -> jsonValue {
					// next token is the key
					return parse_and_eval_expression_key(jsonElement, tokens, ++position, startElement);
				},
				[&](const left_bracket&) -> jsonValue {
					// this is a subscript operator
					return parse_and_eval_expression_subscript(jsonElement, tokens, position, startElement);
				},
				[](const right_bracket&) -> jsonValue { throw std::runtime_error("Invalid expression!"); },
				[](const comma&) -> jsonValue { throw std::runtime_error("Invalid expression!"); },
				[](const left_par&) -> jsonValue { throw std::runtime_error("Invalid expression!"); },
				[&](const right_par&) -> jsonValue { 
					// we can only get here if it is a function that has only one argument, which is an array (otherwise exception is thrown)
					return std::get<jsonArray>(jsonElement);
				},
				[&](const intrinsicFunc&) -> jsonValue { 
					return parse_and_eval_expression_func(jsonElement, tokens, position, startElement);
				},
				[&](const size_function&) -> jsonValue { 
					return parse_and_eval_expression_size(jsonElement, tokens, position, startElement);
				}
				}, tokens[position]);
	}
}

#include "..\h\espression_type_checks.h"

namespace expression_tokenizer {


	std::unordered_map<char, std::function<void(const std::string&, int&, std::vector<expressionToken>&)>> typeMap {
		{'(', add_lpar}, {')', add_rpar}, {'[', add_lbracket}, {']', add_rbracket}, {',', add_comma}, {'.', add_dot}
	};


	void add_lbracket(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens) {
		expressionTokens.emplace_back(left_bracket());
		++position;
	}


	void add_rbracket(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens) {
		expressionTokens.emplace_back(right_bracket());
		++position;
	}


	void add_comma(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens) {
		expressionTokens.emplace_back(comma());
		++position;
	}


	bool check_number_and_add(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens) {
		int startPosition = position;
		// number is valid if , or [ or ( precedes it
		if (startPosition && (str[startPosition - 1] != ',' && str[startPosition - 1] != '[' && str[startPosition - 1] != '(')) {
			return false;
		}
		char c = str[position];
		std::string number = "";
		bool isDouble = false;
		while ((isdigit(c) || c == '-' || c == '.' || c == 'e' || c == 'E' || c == '+')) {
			if (!isdigit(c) && c != '-') {
				isDouble = true;
			}
			number += c;
			++position;
			if (position == str.size()) {
				if (!startPosition) {
					// number is also valid if it is the only element in expression
					if (isDouble) {
						expressionTokens.emplace_back(std::stod(number));
					}
					else {
						expressionTokens.emplace_back(std::stoi(number));
					}
					return true;
				}
				else {
					position = startPosition;
					return false;
				}
			}
			c = str[position];
		}
		if (number.size()) {
			// number is valid if it is followed by , or ] or )
			if (c != ',' && c != ']' && c != ')') {
				position = startPosition;
				return false;
			}
			if (isDouble) {
				expressionTokens.emplace_back(std::stod(number));
			}
			else {
				expressionTokens.emplace_back(std::stoi(number));
			}
			return true;
		}
		return false;
	}


	void add_dot(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens) {
		expressionTokens.emplace_back(dot());
		++position;
	}


	void add_lpar(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens) {
		expressionTokens.emplace_back(left_par());
		++position;
	}


	void add_rpar(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens) {
		expressionTokens.emplace_back(right_par());
		++position;
	}


	bool check_string_or_func_and_add(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens) {
		char c = str[position];
		std::string curr_str = "";
		while (std::isalnum(c)) {
			curr_str += c;
			c = str[++position];
		}
		if (curr_str == "") {
			return false;
		}
		if (c == '(') {
			// this is a function
			if (curr_str == "max") {
				expressionTokens.emplace_back(intrinsic_functions::max);
			}
			else if (curr_str == "min") {
				expressionTokens.emplace_back(intrinsic_functions::min);
			}
			else if (curr_str == "avg") {
				expressionTokens.emplace_back(intrinsic_functions::avg);
			}
			else if (curr_str == "sum") {
				expressionTokens.emplace_back(intrinsic_functions::sum);
			}
			else if (curr_str == "size") {
				expressionTokens.emplace_back(size_function());
			}
			else {
				throw std::runtime_error("Invalid expression!");
			}
		}
		else {
			expressionTokens.emplace_back(curr_str);
		}
		return true;
	}
}

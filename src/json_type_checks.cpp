#include "..\h\json_type_checks.h"

namespace json_tokenizer {


	std::unordered_map<char, std::function<void(const std::string&, int&, std::vector<jsonToken>&)>> typeMap {
		{'{', add_lbrace}, {'}', add_rbrace}, {'[', add_lbracket}, {']', add_rbracket}, {',', add_comma},
		{':', add_colon}, {'"', check_string_and_add}, {'t', check_bool_and_add}, {'f', check_bool_and_add}, {'n', check_null_and_add}
	};


	void add_lbrace(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens) {
		jsonTokens.emplace_back(left_brace());
		++position;
	}


	void add_rbrace(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens) {
		jsonTokens.emplace_back(right_brace());
		++position;
	}


	void add_lbracket(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens) {
		jsonTokens.emplace_back(left_bracket());
		++position;
	}


	void add_rbracket(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens) {
		jsonTokens.emplace_back(right_bracket());
		++position;
	}


	void add_comma(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens) {
		jsonTokens.emplace_back(comma());
		++position;
	}


	void add_colon(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens) {
		jsonTokens.emplace_back(colon());
		++position;
	}


	void check_number_and_add(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens) {
		char c = str[position];
		std::string number = "";
		bool isDouble = false;
		while ((isdigit(c) || c == '-' || c == '.' || c == 'e' || c == 'E' || c == '+')) {
			if (!isdigit(c) && c != '-') {
				isDouble = true;
			}
			number += c;
			c = str[++position];
		}
		if (number.size()) {
			if (isDouble) {
				jsonTokens.emplace_back(std::stod(number));
			}
			else {
				jsonTokens.emplace_back(std::stoi(number));
			}
		}
		else {
			throw std::runtime_error("Invalid JSON!");
		}
	}


	void check_string_and_add(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens) {
		char c = str[++position];
		std::string curr_str = "";
		bool escaped = false; // to check if " is escaped
		while (c != '"' || escaped) {
			if (c == '\\') {
				escaped = true;
			}
			else {
				escaped = false;
			}
			curr_str += c;
			c = str[++position];
		}
		++position;
		jsonTokens.emplace_back(curr_str);
	}


	void check_bool_and_add(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens) {
		if (str.substr(position, 4) == "true") {
			jsonTokens.emplace_back(true);
			position += 4;
		}
		else if (str.substr(position, 5) == "false") {
			jsonTokens.emplace_back(false);
			position += 5;
		}
		else {
			throw std::runtime_error("Invalid JSON!");
		}
	}


	void check_null_and_add(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens) {
		if (str.substr(position, 4) == "null") {
			jsonTokens.emplace_back(null_value());
			position += 4;
		}
		else {
			throw std::runtime_error("Invalid JSON!");
		}
	}
}

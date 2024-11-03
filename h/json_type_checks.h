#pragma once
#include <functional>

#include "shared_structures.h"

namespace json_tokenizer {

    // functions for checking token types and adding them to a vector

    void add_lbrace(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens);
    void add_rbrace(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens);
    void add_lbracket(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens);
    void add_rbracket(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens);
    void add_comma(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens);
    void add_colon(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens);
    void check_string_and_add(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens);
    void check_bool_and_add(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens);
    void check_null_and_add(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens);
    void check_number_and_add(const std::string& str, int& position, std::vector<jsonToken>& jsonTokens);

    // unordered map used for faster type checking (p.e. " means that we should check for a valid string)
    extern std::unordered_map<char, std::function<void(const std::string&, int&, std::vector<jsonToken>&)>> typeMap;

}
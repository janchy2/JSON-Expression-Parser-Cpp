#pragma once
#include "shared_structures.h"

namespace expression_tokenizer {

    // functions for checking token types and adding them to a vector

    void add_lbracket(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens);
    void add_rbracket(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens);
    void add_comma(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens);
    void add_dot(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens);
    void add_lpar(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens);
    void add_rpar(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens);
    bool check_string_or_func_and_add(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens);
    bool check_number_and_add(const std::string& str, int& position, std::vector<expressionToken>& expressionTokens);

    // unordered map used for faster type checking
    extern std::unordered_map<char, std::function<void(const std::string&, int&, std::vector<expressionToken>&)>> typeMap;

}
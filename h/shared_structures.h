#pragma once
#include <variant>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>

#include "intrinsic_functions.h"

    
// empty structs that are just used for recognizing token types
struct left_brace {};
struct right_brace {};
struct left_bracket {};
struct right_bracket {};
struct null_value {};
struct comma {};
struct colon {};
struct dot {};
struct left_par {};
struct right_par {};
struct size_function {};

// every type that a JSON token can be
using valueToken = std::variant< null_value, std::string, double, int, bool>;
using jsonToken = std::variant < left_brace, right_brace, left_bracket, right_bracket, comma, colon, valueToken >;

// wrapper struct to avoid direct recursion in the definition od jsonValue variant
struct jsonValueWrapper;

using jsonArray = std::vector<jsonValueWrapper>;
using jsonObject = std::unordered_map<std::string, jsonValueWrapper>;

// every type that a JSON value can be
using jsonValue = std::variant<jsonArray, jsonObject, valueToken>;

struct jsonValueWrapper {
    jsonValue value;
    jsonValueWrapper() {}
    jsonValueWrapper(const jsonValue& val) : value(val) {}
    jsonValueWrapper(jsonValue&& val) noexcept : value(std::move(val)) {}
    jsonValueWrapper(const jsonValueWrapper& other) : value(other.value) {}
    jsonValueWrapper(jsonValueWrapper&& other) noexcept : value(std::move(other.value)) {}
    jsonValueWrapper& operator=(const jsonValueWrapper& other) {
        if (this != &other) {
            value = other.value;
        }
        return *this;
    }
    jsonValueWrapper& operator=(jsonValueWrapper&& other) noexcept {
        if (this != &other) {
            value = std::move(other.value);
        }
        return *this;
    }
};

using intrinsicFunc = std::function<intrinsic_functions::number(std::vector<intrinsic_functions::number>&)>;

// every value that an expression token can be
using expressionToken = std::variant< std::string, int, double, dot, left_bracket, right_bracket, comma, left_par, right_par,
    intrinsicFunc, size_function >;

// overloaded class used for visit of variant
template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

// deduction guide
template<class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

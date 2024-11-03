#include <iostream>
#include <fstream>
#include <future>
#include <regex>

#include "..\h\tokenizer.h"
#include "..\h\parser.h"
#include "..\h\operators.h"
#include "..\h\testing_header.h"


std::string read_json_without_whitespaces(std::ifstream& inputFile) {
    // reads the json file and removes whitespaces
    std::string cleanJson;
    char c;
    bool isPartOfString = false; // to check if whitespace is part of string
    bool escaped = false; // to check if " is escaped
    while (inputFile.get(c)) {
        if (c == '\\') {
            escaped = true;
        }
        else {
            if (c == '"' and !escaped) {
                isPartOfString = !isPartOfString;
            }
            escaped = false;
        }
        if (!std::isspace(c) or isPartOfString) {
            cleanJson += c;
        }
    }
    return cleanJson;
}


std::string remove_expression_whitespaces(const std::string& expression) {
    std::string result = expression;
    result.erase(std::remove_if(result.begin(), result.end(), [](unsigned char c) {
        return std::isspace(c);
        }), result.end());
    return result;
}


void split_expression_with_operators(const std::string& expression, std::vector<std::string>& operands, std::vector<char>& operators) {
    // regex that matches operators +,-,*,/ and operands
    std::regex re("([+\\-*/])|([^+\\-*/]+)");
    std::sregex_iterator it(expression.begin(), expression.end(), re);
    std::sregex_iterator end;

    for (; it != end; ++it) {
        if ((*it)[1].matched) {
            // the operator is matched
            operators.emplace_back((*it)[1].str()[0]);
        }
        else if ((*it)[2].matched) {
            // the operand is matched
            operands.emplace_back((*it)[2].str());
        }
    }
}


jsonValue evaluate_operator_expression(jsonValue& jsonStructure, const std::vector<std::string> operands, const std::vector<char> operators) {
    // evaluate each operand using async to try to speed it up

    std::vector<std::future<jsonValue>> asyncResults;
    for (const std::string& operand : operands) {
        // evaluate each operand expression and push its result to asyncResults
        asyncResults.emplace_back(std::async([&] {
            std::vector<expressionToken> tokens = tokenizer::tokenize_expression(remove_expression_whitespaces(operand));
            return parser::parse_and_evaluate_expression(tokens, jsonStructure);
            }));
    }
    std::vector<operators::number> numberOperands;
    // get the results and convert to int or double
    for (std::future<jsonValue>& asyncResult : asyncResults) {
        try {
            valueToken value = std::get<valueToken>(asyncResult.get());
            if (value.index() == 2) {
                // double
                numberOperands.emplace_back(std::get<double>(value));
            }
            else {
                // int or invalid value (throws expression)
                numberOperands.emplace_back(std::get<int>(value));
            }
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Invalid expression!");
        }
    }
    // calculate operator expression and return
    operators::number operatorsResult = operators::calculate_operators(numberOperands, operators);
    return std::visit(overloaded{
        [](int& i) -> jsonValue {
            return i;
        },
        [](double& d) -> jsonValue {
            return d;
        }
        }, operatorsResult);
}


jsonValue evaluate_expression(const std::string& expression, jsonValue& jsonStructure) {
    std::vector<std::string> operands;
    std::vector<char> operators;
    split_expression_with_operators(expression, operands, operators);
    if (!operands.size() && !operators.size()) {
        // empty expression
        return jsonStructure;
    }
    if (operands.size() == 1 && !operators.size()) {
        // no operators in expression, just evaluate it
        std::vector<expressionToken> tokens = tokenizer::tokenize_expression(remove_expression_whitespaces(operands[0]));
        return parser::parse_and_evaluate_expression(tokens, jsonStructure);
    }
    if (operands.size() != operators.size() + 1) {
        throw std::runtime_error("Invalid expression!");
    }
    return evaluate_operator_expression(jsonStructure, operands, operators);
}


void printJsonValue(const jsonValue& value) {
    std::visit(overloaded{
        [](const jsonObject& obj) {
            std::cout << '{';
            bool first = true;
            for (const auto& pair : obj) {
                if (!first) {
                    std::cout << ", ";
                }
                else {
                    first = false;
                }
                std::cout << '"' << pair.first << '"' << " : ";
                printJsonValue(pair.second.value);
            }
            std::cout << '}';
        },
        [](const jsonArray& arr) {
            std::cout << '[';
            bool first = true;
            for (const jsonValueWrapper& elem : arr) {
                if (!first) {
                    std::cout << ", ";
                }
                else {
                    first = false;
                }
                printJsonValue(elem.value);
            }
            std::cout << ']';
        },
        [](const valueToken& val) {
            std::visit(overloaded {
                [](const null_value&) { std::cout << "null"; },
                [](const std::string& s) { std::cout << s; },
                [](const double& d) { std::cout << d; },
                [](const int& i) { std::cout << i; },
                [](const bool& b) { std::cout << b; },
            }, val);
        }
        }, value);
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Error: No file path specified!" << std::endl;
        return 1;
    }

    if (argc < 3) {
        std::cerr << "Error: No expression specified!" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::ifstream inputFile(inputFilePath);
    if (!inputFile) {
        std::cerr << "Error: Could not open the JSON file!" << std::endl;
        return 2;
    }
    std::string jsonString = read_json_without_whitespaces(inputFile);
    inputFile.close();

    try {
        std::vector<jsonToken> tokens = tokenizer::tokenize_json(jsonString);
        jsonValue jsonStructure = parser::parse_json(tokens);
        std::string expression = argv[2];
        jsonValue result = evaluate_expression(expression, jsonStructure);
        std::cout << std::boolalpha;
        printJsonValue(result);
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 3;
    }
}

#include "..\h\operators.h"

namespace operators {

	
	number add(number n1, number n2) {
		return std::visit([](const auto& first, const auto& scnd) -> number {
			return first + scnd;
			}, n1, n2);
	}


	number subtract(number n1, number n2) {
		return std::visit([](const auto& first, const auto& scnd) -> number {
			return first - scnd;
			}, n1, n2);
	}


	number multiply(number n1, number n2) {
		return std::visit([](const auto& first, const auto& scnd) -> number {
			return first * scnd;
			}, n1, n2);
	}


	number divide(number n1, number n2) {
		return std::visit([](const auto& first, const auto& scnd) -> number {
			return static_cast<double>(first) / scnd;
			}, n1, n2);
	}


	std::unordered_map<char, std::function<number(number, number)>> operatorMap{
		{'+', add}, {'-', subtract}, {'*', multiply}, {'/', divide}
	};


	number calculate_operators(const std::vector<number>& operands, const std::vector<char>& operators) {
		std::stack<number> operandStack;
		std::stack<char> operatorStack;
		operandStack.push(operands[0]);
		// we need to iterate twice because of operator priority (we calculate * and / first)
		for (int i = 0; i < operators.size(); ++i) {
			char op = operators[i];
			if (op == '*' || op == '/') {
				number first = operandStack.top();
				operandStack.pop();
				operandStack.push(operatorMap[op](first, operands[i + 1]));
			}
			else {
				operandStack.push(operands[i + 1]);
				operatorStack.push(op);
			}
		}
		while (!operatorStack.empty()) {
			number scnd = operandStack.top();
			operandStack.pop();
			number first = operandStack.top();
			operandStack.pop();
			char op = operatorStack.top();
			operatorStack.pop();
			operandStack.push(operatorMap[op](first, scnd));
		}
		return operandStack.top();
	}
}

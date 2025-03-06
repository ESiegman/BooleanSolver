#include "Utils.hpp"
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <unordered_set>
#include <algorithm>

int apply_op(int lhs, int rhs, const std::string& op) {
    std::cout << "Applying operation: " << lhs << " " << op << " " << rhs << std::endl;
    if (op == "and") return lhs && rhs;
    if (op == "or") return lhs || rhs;
    if (op == "xor") return lhs ^ rhs;
    if (op == "nand") return !(lhs && rhs);
    throw std::invalid_argument("Invalid operator: " + op);
}

int precedence(const std::string& op) {
    if (op == "or") return 1;
    if (op == "and") return 2;
    if (op == "xor") return 3;
    if (op == "not") return 4;
    if (op == "nand") return 5;
    return 0;
}

std::vector<std::string> tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string current_token;

    for (char ch : expr) {
        if (std::isspace(ch)) {
            continue;
        } else if (std::isalpha(ch)) {
            current_token += ch;
        } else {
            if (!current_token.empty()) {
                tokens.push_back(current_token);
                current_token.clear();
            }
            if (ch == '(' || ch == ')') {
                tokens.push_back(std::string(1, ch));
            } else {
                current_token += ch;
                if (current_token == "and" || current_token == "or" || current_token == "xor" || current_token == "not" || current_token == "nand") {
                    tokens.push_back(current_token);
                    current_token.clear();
                }
            }
        }
    }
    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }
    return tokens;
}

void process_operator(std::stack<int>& operands, std::stack<std::string>& operators) {
    std::string op = operators.top(); operators.pop();
    int rhs = operands.top(); operands.pop();
    int lhs = operands.top(); operands.pop();
    int result = apply_op(lhs, rhs, op);
    std::cout << lhs << " " << op << " " << rhs << " = " << result << std::endl;
    operands.push(result);
}

int evaluateExpression(const std::string& expr, const std::map<char, int>& variables) {
    auto tokens = tokenize(expr);
    std::stack<std::string> operators;
    std::stack<int> operands;

    for (const auto& token : tokens) {
        try {
            if (std::isalpha(token[0])) {
                int value = variables.at(token[0]);
                operands.push(value);
                std::cout << "Pushing variable: " << token[0] << " = " << value << std::endl;
            } else if (token == "(") {
                operators.push(token);
                std::cout << "Pushing operator: " << token << std::endl;
            } else if (token == ")") {
                while (!operators.empty() && operators.top() != "(") {
                    process_operator(operands, operators);
                }
                if (!operators.empty()) {
                    std::cout << "Popping operator: " << operators.top() << std::endl;
                    operators.pop();
                }
            } else if (token == "not") {
                operators.push(token);
                std::cout << "Pushing operator: " << token << std::endl;
            } else if (token == "and" || token == "or" || token == "xor" || token == "nand") {
                while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                    process_operator(operands, operators);
                }
                operators.push(token);
                std::cout << "Pushing operator: " << token << std::endl;
            } else {
                throw std::invalid_argument("Invalid token: " + token);
            }
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: Variable " << token << " not found in the provided map. " << e.what() << std::endl;
            throw;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            throw;
        }
    }

    while (!operators.empty()) {
        process_operator(operands, operators);
    }

    if (operands.empty()) {
        throw std::invalid_argument("Invalid expression: " + expr);
    }

    return operands.top();
}

std::unordered_set<char> extractVariables(const std::string& booleanExpression) {
    std::unordered_set<char> variables;
    for (char ch : booleanExpression) {
        if (std::isupper(ch)) {
            variables.insert(ch);
        }
    }
    return variables;
}

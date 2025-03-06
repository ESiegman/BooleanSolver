#include <iostream>
#include <stack>
#include <stdexcept>
#include <unordered_set>
#include <map>
#include <vector>
#include <cctype>
#include <sstream>

// Function to apply a boolean operation
int apply_op(int lhs, int rhs, const std::string& op) {
    if (op == "AND" || op == "and") return lhs && rhs;
    if (op == "OR" || op == "or") return lhs || rhs;
    if (op == "XOR" || op == "xor") return lhs ^ rhs;
    throw std::invalid_argument("Invalid operator");
}

// Function to determine the precedence of operators
int precedence(const std::string& op) {
    if (op == "AND" || op == "and") return 3;
    if (op == "OR" || op == "or") return 2;
    if (op == "XOR" || op == "xor") return 1;
    return 0;
}

// Function to tokenize a boolean expression
std::vector<std::string> tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string token;
    for (size_t i = 0; i < expr.size(); ++i) {
        if (std::isspace(expr[i])) continue;
        if (std::isalpha(expr[i])) {
            token += expr[i];
            while (i + 1 < expr.size() && std::isalpha(expr[i + 1])) {
                token += expr[++i];
            }
            tokens.push_back(token);
            token = "";
        } else if (std::isdigit(expr[i]) || expr[i] == '0' || expr[i] == '1') {
            tokens.push_back(std::string(1, expr[i]));
        } else {
            tokens.push_back(std::string(1, expr[i]));
        }
    }
    return tokens;
}

// Function to extract variables from a boolean expression
std::unordered_set<char> extractVariables(const std::string& booleanExpression) {
    std::unordered_set<char> variables;
    for (char ch : booleanExpression) {
        if (std::isalpha(ch) && std::isupper(ch)) {
            variables.insert(ch);
        }
    }
    return variables;
}

// Main function to evaluate a boolean expression
int evaluateExpression(const std::string& expr, const std::map<char, int>& variables) {
    std::vector<std::string> tokens = tokenize(expr);
    std::stack<int> values;
    std::stack<std::string> ops;

    for (const std::string& token : tokens) {
        if (std::isalpha(token[0]) && std::isupper(token[0])) {
            if (variables.find(token[0]) == variables.end()) {
                throw std::invalid_argument("Variable not found in the provided map");
            }
            values.push(variables.at(token[0]));
        } else if (token == "(") {
            ops.push(token);
        } else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                int rhs = values.top(); values.pop();
                int lhs = values.top(); values.pop();
                std::string op = ops.top(); ops.pop();
                values.push(apply_op(lhs, rhs, op));
            }
            if (!ops.empty()) ops.pop();
        } else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(token)) {
                int rhs = values.top(); values.pop();
                int lhs = values.top(); values.pop();
                std::string op = ops.top(); ops.pop();
                values.push(apply_op(lhs, rhs, op));
            }
            ops.push(token);
        }
    }

    while (!ops.empty()) {
        int rhs = values.top(); values.pop();
        int lhs = values.top(); values.pop();
        std::string op = ops.top(); ops.pop();
        values.push(apply_op(lhs, rhs, op));
    }

    return values.top();
}

#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <map>
#include <unordered_set>

int apply_op(int lhs, int rhs, const std::string& op);
int precedence(const std::string& op);
std::vector<std::string> tokenize(const std::string& expr);
int evaluateExpression(const std::string& expr, const std::map<char, int>& variables);
std::unordered_set<char> extractVariables(const std::string& booleanExpression);

#endif // UTILS_HPP

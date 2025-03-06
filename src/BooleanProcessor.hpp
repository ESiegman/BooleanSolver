#ifndef BOOLEANPROCESSOR_HPP
#define BOOLEANPROCESSOR_HPP

#include <vector>
#include <string>

class BooleanProcessor {
public:
    static std::vector<std::vector<int>> booleanToTruthTable(const std::string& booleanExpression);
    static std::vector<std::vector<int>> truthTableToKMap(const std::vector<std::vector<int>>& truthTable);
    static void printTruthTable(const std::vector<std::vector<int>>& truthTable);
    static void printKMap(const std::vector<std::vector<int>>& kmap);
    static std::vector<std::vector<int>> generateTruthTable(int numVars);
private:
    static int evaluateBooleanExpression(const std::string& booleanExpression, const std::vector<int>& variables);
};

#endif // BOOLEANPROCESSOR_HPP

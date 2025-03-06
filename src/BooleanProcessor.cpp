#include "BooleanProcessor.hpp"
#include "Utils.hpp"
#include <iostream>
#include <bitset>
#include <cmath>
#include <map>
#include <algorithm>

std::vector<std::vector<int>> BooleanProcessor::booleanToTruthTable(const std::string& booleanExpression) {
    auto variables = extractVariables(booleanExpression);
    int numVars = variables.size();
    int numRows = std::pow(2, numVars);
    std::vector<std::vector<int>> truthTable(numRows, std::vector<int>(numVars + 1));

    std::vector<char> varList(variables.begin(), variables.end());
    std::sort(varList.begin(), varList.end());

    for (int i = 0; i < numRows; ++i) {
        std::bitset<32> bits(i);
        std::map<char, int> varMap;
        for (int j = 0; j < numVars; ++j) {
            varMap[varList[j]] = bits[numVars - j - 1];
            truthTable[i][j] = bits[numVars - j - 1];
        }
        try {
            for (const auto& var : varMap) {
            }
            truthTable[i][numVars] = evaluateExpression(booleanExpression, varMap);
        } catch (const std::exception& e) {
            throw;
        }
    }
    return truthTable;
}


int BooleanProcessor::evaluateBooleanExpression(const std::string& booleanExpression, const std::vector<int>& variables) {
    auto varSet = extractVariables(booleanExpression);
    std::vector<char> varList(varSet.begin(), varSet.end());
    std::sort(varList.begin(), varList.end());

    std::map<char, int> varMap;
    for (size_t i = 0; i < varList.size(); ++i) {
        varMap[varList[i]] = variables[i];
    }

    try {
        return evaluateExpression(booleanExpression, varMap);
    } catch (const std::exception& e) {
        std::cerr << "Error evaluating Boolean expression: " << e.what() << std::endl;
        throw;
    }
}

std::vector<std::vector<int>> BooleanProcessor::truthTableToKMap(const std::vector<std::vector<int>>& truthTable) {
    int size = truthTable.size();
    int numVars = truthTable[0].size() - 1;
    int kmapSize = sqrt(size);

    std::vector<std::vector<int>> kmap(kmapSize, std::vector<int>(kmapSize));

    auto binaryToGray = [](int num) {
        return num ^ (num >> 1);
    };
    
    for (int i = 0; i < size; ++i) {
        int rowIndex = binaryToGray(i / kmapSize);
        int colIndex = binaryToGray(i % kmapSize);
        kmap[rowIndex][colIndex] = truthTable[i][numVars];
    }

    return kmap;
}

void BooleanProcessor::printTruthTable(const std::vector<std::vector<int>>& truthTable) {
    std::cout << "Inputs | Output" << std::endl;
    std::cout << "----------------" << std::endl;
    for (const auto& row : truthTable) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (i == row.size() - 1)
                std::cout << "| ";
            std::cout << row[i] << " ";
        }
        std::cout << std::endl;
    }
}

void BooleanProcessor::printKMap(const std::vector<std::vector<int>>& kmap) {
    std::cout << "K-map:" << std::endl;
    for (const auto& row : kmap) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> BooleanProcessor::generateTruthTable(int numVars) {
    int numRows = 1 << numVars;
    std::vector<std::vector<int>> truthTable(numRows, std::vector<int>(numVars + 1));

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numVars; ++j) {
            truthTable[i][j] = (i >> (numVars - j - 1)) & 1;
        }
    }

    return truthTable;
}

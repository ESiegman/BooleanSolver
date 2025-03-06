#include "KMapSolver.hpp"
#include "BooleanProcessor.hpp"
#include <iostream>

void printMenu() {
    std::cout << "Select an option:" << std::endl;
    std::cout << "1. Boolean expression to Truth Table" << std::endl;
    std::cout << "2. Boolean expression to K-map and POS/SOP" << std::endl;
    std::cout << "3. Truth Table to K-map and POS/SOP" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    int choice;
    printMenu();
    std::cin >> choice;

    std::string booleanExpression;
    std::vector<std::vector<int>> truthTable;
    const int numVars = 4; // Assuming 4 variables for option 3

    switch (choice) {
        case 1:
            std::cout << "Enter the Boolean expression: ";
            std::cin.ignore();
            std::getline(std::cin, booleanExpression);
            truthTable = BooleanProcessor::booleanToTruthTable(booleanExpression);
            BooleanProcessor::printTruthTable(truthTable);
            break;
        case 2:
            std::cout << "Enter the Boolean expression: ";
            std::cin.ignore();
            std::getline(std::cin, booleanExpression);
            truthTable = BooleanProcessor::booleanToTruthTable(booleanExpression);
            {
                auto kmap = BooleanProcessor::truthTableToKMap(truthTable);
                BooleanProcessor::printKMap(kmap);
                KMapSolver solver(kmap);
                auto [sop_result, pos_result] = solver.kmapToSOPandPOS();
                std::cout << "SOP: " << sop_result << std::endl;
                std::cout << "POS: " << pos_result << std::endl;
            }
            break;
        case 3:
            truthTable = BooleanProcessor::generateTruthTable(numVars);
            for (int i = 0; i < (1 << numVars); ++i) {
                for (int j = 0; j < numVars; ++j) {
                    std::cout << truthTable[i][j] << " ";
                }
                std::cout << ": Output for row " << i + 1 << ": ";
                std::cin >> truthTable[i][numVars];
            }
            {
                auto kmap = BooleanProcessor::truthTableToKMap(truthTable);
                BooleanProcessor::printKMap(kmap);
                KMapSolver solver(kmap);
                auto [sop_result, pos_result] = solver.kmapToSOPandPOS();
                std::cout << "SOP: " << sop_result << std::endl;
                std::cout << "POS: " << pos_result << std::endl;
            }
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            break;
    }

    return 0;
}

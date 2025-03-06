#include "KMapSolver.hpp"
#include <iostream>
#include <stdexcept>
#include <set>

// Constructor
KMapSolver::KMapSolver(const std::vector<std::vector<int>>& kmap) : kmap(kmap) {
    numVars = numVariables();
    variables = getVariableNames(numVars);
}

// Determine the number of variables based on the K-map size
int KMapSolver::numVariables() {
    int rows = kmap.size();
    int cols = kmap[0].size();
    if (rows == 2 && cols == 2) {
        return 2;
    } else if (rows == 2 && cols == 4) {
        return 3;
    } else if (rows == 4 && cols == 4) {
        return 4;
    } else {
        throw std::invalid_argument("Invalid K-map size");
    }
}

// Get variable names based on the number of variables
std::vector<std::string> KMapSolver::getVariableNames(int num_vars) {
    if (num_vars == 2) {
        return {"A", "B"};
    } else if (num_vars == 3) {
        return {"A", "B", "C"};
    } else if (num_vars == 4) {
        return {"A", "B", "C", "D"};
    } else {
        throw std::invalid_argument("Invalid number of variables");
    }
}

// Generate term from rows and columns
std::string KMapSolver::getTerm(const std::set<int>& rows, const std::set<int>& cols, bool isSOP) {
    std::vector<std::string> terms;

    // Handle the rows (A, B)
    if (numVars >= 3) {
        if (rows.size() == 2) {
            auto it = rows.begin();
            int first = *it;
            int second = *(++it);
            if ((first == 0 && second == 1) || (first == 2 && second == 3)) {
                // Change in B only
                terms.push_back(variables[1] + (isSOP ? "" : "'"));
            } else if ((first == 0 && second == 3) || (first == 1 && second == 2)) {
                // Change in A only
                terms.push_back(variables[0] + (isSOP ? "" : "'"));
            }
        } else if (rows.size() == 1) {
            int row = *rows.begin();
            if ((row & 2) == 0) {
                terms.push_back(variables[0] + (isSOP ? "'" : ""));
            } else {
                terms.push_back(variables[0] + (isSOP ? "" : "'"));
            }
            if ((row & 1) == 0) {
                terms.push_back(variables[1] + (isSOP ? "'" : ""));
            } else {
                terms.push_back(variables[1] + (isSOP ? "" : "'"));
            }
        }
    }

    // Handle the columns (C, D)
    if (numVars == 4) {
        if (cols.size() == 2) {
            auto it = cols.begin();
            int first = *it;
            int second = *(++it);
            if ((first == 0 && second == 1) || (first == 2 && second == 3)) {
                // Change in D only
                terms.push_back(variables[3] + (isSOP ? "" : "'"));
            } else if ((first == 0 && second == 3) || (first == 1 && second == 2)) {
                // Change in C only
                terms.push_back(variables[2] + (isSOP ? "" : "'"));
            }
        } else if (cols.size() == 1) {
            int col = *cols.begin();
            if ((col & 2) == 0) {
                terms.push_back(variables[2] + (isSOP ? "'" : ""));
            } else {
                terms.push_back(variables[2] + (isSOP ? "" : "'"));
            }
            if ((col & 1) == 0) {
                terms.push_back(variables[3] + (isSOP ? "'" : ""));
            } else {
                terms.push_back(variables[3] + (isSOP ? "" : "'"));
            }
        }
    }

    std::string term;
    if (isSOP) {
        for (const auto& t : terms) {
            term += t;
        }
    } else {
        for (size_t i = 0; i < terms.size(); ++i) {
            if (i > 0) term += "+";
            term += terms[i];
        }
    }

    return term;
}

// Check if a cell is in a group
bool KMapSolver::isInGroup(const std::vector<std::pair<int, int>>& group, int r, int c) {
    for (const auto& cell : group) {
        if (cell.first == r && cell.second == c) {
            return true;
        }
    }
    return false;
}

// Check if a cell is inside a 2x2 group
bool KMapSolver::isInside2x2Group(int r, int c) {
    for (const auto& grouping : twoByTwoGroupings) {
        if (isInGroup(grouping.cells, r, c)) {
            return true;
        }
    }
    return false;
}

// Find groupings
void KMapSolver::findGroupings(bool isSOP) {
    int rows = kmap.size();
    int cols = kmap[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    auto getWrappedIndex = [&](int index, int maxIndex) {
        return (index + maxIndex) % maxIndex;
    };

    // Check for corner wrap-around groupings
    if ((isSOP && kmap[0][0] == 1 && kmap[0][cols - 1] == 1 && kmap[rows - 1][0] == 1 && kmap[rows - 1][cols - 1] == 1) ||
        (!isSOP && kmap[0][0] == 0 && kmap[0][cols - 1] == 0 && kmap[rows - 1][0] == 0 && kmap[rows - 1][cols - 1] == 0)) {
        Grouping grouping;
        std::set<int> groupRows = {0, rows - 1};
        std::set<int> groupCols = {0, cols - 1};
        grouping.cells.push_back({0, 0});
        grouping.cells.push_back({0, cols - 1});
        grouping.cells.push_back({rows - 1, 0});
        grouping.cells.push_back({rows - 1, cols - 1});
        grouping.term = getTerm(groupRows, groupCols, isSOP);
        if (isSOP) {
            sopGroupings.push_back(grouping);
        } else {
            posGroupings.push_back(grouping);
        }
        twoByTwoGroupings.push_back(grouping);
        visited[0][0] = visited[0][cols - 1] = visited[rows - 1][0] = visited[rows - 1][cols - 1] = true;
    }

    // Find 2x2 groupings
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!visited[r][c] && ((isSOP && kmap[r][c] == 1) || (!isSOP && kmap[r][c] == 0))) {
                int rNext = getWrappedIndex(r + 1, rows);
                int cNext = getWrappedIndex(c + 1, cols);
                if (kmap[r][c] == kmap[rNext][c] && kmap[r][c] == kmap[r][cNext] && kmap[r][c] == kmap[rNext][cNext]) {
                    Grouping grouping;
                    std::set<int> groupRows = {r, rNext};
                    std::set<int> groupCols = {c, cNext};
                    grouping.cells.push_back({r, c});
                    grouping.cells.push_back({rNext, c});
                    grouping.cells.push_back({r, cNext});
                    grouping.cells.push_back({rNext, cNext});
                    grouping.term = getTerm(groupRows, groupCols, isSOP);
                    if (isSOP) {
                        sopGroupings.push_back(grouping);
                    } else {
                        posGroupings.push_back(grouping);
                    }
                    twoByTwoGroupings.push_back(grouping);
                    visited[r][c] = visited[rNext][c] = visited[r][cNext] = visited[rNext][cNext] = true;
                }
            }
        }
    }

    // Find 1x2 and 2x1 groupings that can group with already grouped cells
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!visited[r][c] && ((isSOP && kmap[r][c] == 1) || (!isSOP && kmap[r][c] == 0))) {
                int rNext = getWrappedIndex(r + 1, rows);
                int cNext = getWrappedIndex(c + 1, cols);

                if (kmap[r][c] == kmap[rNext][c]) {
                    Grouping grouping;
                    std::set<int> groupRows = {r, rNext};
                    std::set<int> groupCols = {c};
                    grouping.cells.push_back({r, c});
                    grouping.cells.push_back({rNext, c});
                    grouping.term = getTerm(groupRows, groupCols, isSOP);
                    if (isSOP) {
                        sopGroupings.push_back(grouping);
                    } else {
                        posGroupings.push_back(grouping);
                    }
                    visited[r][c] = visited[rNext][c] = true;
                } else if (kmap[r][c] == kmap[r][cNext]) {
                    Grouping grouping;
                    std::set<int> groupRows = {r};
                    std::set<int> groupCols = {c, cNext};
                    grouping.cells.push_back({r, c});
                    grouping.cells.push_back({r, cNext});
                    grouping.term = getTerm(groupRows, groupCols, isSOP);
                    if (isSOP) {
                        sopGroupings.push_back(grouping);
                    } else {
                        posGroupings.push_back(grouping);
                    }
                    visited[r][c] = visited[r][cNext] = true;
                }

                // Try to group with cells already in a group
                if (rNext < rows && !visited[rNext][c] && isInGroup(sopGroupings.back().cells, rNext, c)) {
                    sopGroupings.back().cells.push_back({r, c});
                    sopGroupings.back().term = getTerm({r, rNext}, {c}, isSOP);
                    visited[r][c] = true;
                } else if (r - 1 >= 0 && !visited[r - 1][c] && isInGroup(sopGroupings.back().cells, r - 1, c)) {
                    sopGroupings.back().cells.push_back({r, c});
                    sopGroupings.back().term = getTerm({r - 1, r}, {c}, isSOP);
                    visited[r][c] = true;
                } else if (cNext < cols && !visited[r][cNext] && isInGroup(sopGroupings.back().cells, r, cNext)) {
                    sopGroupings.back().cells.push_back({r, c});
                    sopGroupings.back().term = getTerm({r}, {c, cNext}, isSOP);
                    visited[r][c] = true;
                } else if (c - 1 >= 0 && !visited[r][c - 1] && isInGroup(sopGroupings.back().cells, r, c - 1)) {
                    sopGroupings.back().cells.push_back({r, c});
                    sopGroupings.back().term = getTerm({r}, {c - 1, c}, isSOP);
                    visited[r][c] = true;
                }
            }
        }
    }

    // Find single cell groupings without adding adjacent cells
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!visited[r][c] && !isInside2x2Group(r, c) && ((isSOP && kmap[r][c] == 1) || (!isSOP && kmap[r][c] == 0))) {
                Grouping grouping;
                std::set<int> groupRows = {r};
                std::set<int> groupCols = {c};
                grouping.cells.push_back({r, c});
                grouping.term = getTerm(groupRows, groupCols, isSOP);
                if (isSOP) {
                    sopGroupings.push_back(grouping);
                } else {
                    posGroupings.push_back(grouping);
                }
                visited[r][c] = true;
            }
        }
    }

    // Check for vertical wrap-around groupings with cross-referencing against 2x2 groupings
    for (int c = 0; c < cols; ++c) {
        if (((isSOP && kmap[0][c] == 1 && kmap[rows - 1][c] == 1) ||
             (!isSOP && kmap[0][c] == 0 && kmap[rows - 1][c] == 0)) &&
            !isInside2x2Group(0, c) && !isInside2x2Group(rows - 1, c)) {
            Grouping grouping;
            std::set<int> groupRows = {0, rows - 1};
            std::set<int> groupCols = {c};
            grouping.cells.push_back({0, c});
            grouping.cells.push_back({rows - 1, c});
            grouping.term = getTerm(groupRows, groupCols, isSOP);
            if (isSOP) {
                sopGroupings.push_back(grouping);
            } else {
                posGroupings.push_back(grouping);
            }
            visited[0][c] = visited[rows - 1][c] = true;
        }
    }

    // Check for horizontal wrap-around groupings with cross-referencing against 2x2 groupings
    for (int r = 0; r < rows; ++r) {
        if (((isSOP && kmap[r][0] == 1 && kmap[r][cols - 1] == 1) ||
             (!isSOP && kmap[r][0] == 0 && kmap[r][cols - 1] == 0)) &&
            !isInside2x2Group(r, 0) && !isInside2x2Group(r, cols - 1)) {
            Grouping grouping;
            std::set<int> groupRows = {r};
            std::set<int> groupCols = {0, cols - 1};
            grouping.cells.push_back({r, 0});
            grouping.cells.push_back({r, cols - 1});
            grouping.term = getTerm(groupRows, groupCols, isSOP);
            if (isSOP) {
                sopGroupings.push_back(grouping);
            } else {
                posGroupings.push_back(grouping);
            }
            visited[r][0] = visited[r][cols - 1] = true;
        }
    }
}

// Generate expression with unique terms
std::string KMapSolver::generateExpression(const std::list<Grouping>& groupings, bool isSOP) {
    std::set<std::string> uniqueTerms;
    std::string expression;

    for (const auto& grouping : groupings) {
        if (uniqueTerms.find(grouping.term) == uniqueTerms.end()) {
            if (!expression.empty()) {
                expression += isSOP ? " + " : " ";
            }
            expression += isSOP ? grouping.term : "(" + grouping.term + ")";
            uniqueTerms.insert(grouping.term);
        }
    }

    return expression;
}

// Generate SOP and POS expressions
std::pair<std::string, std::string> KMapSolver::kmapToSOPandPOS() {
    findGroupings(true);  // Find SOP groupings
    findGroupings(false); // Find POS groupings

    std::string sop_result = generateExpression(sopGroupings, true);
    std::string pos_result = generateExpression(posGroupings, false);

    return {sop_result, pos_result};
}

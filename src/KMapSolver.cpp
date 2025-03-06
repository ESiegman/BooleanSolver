#include "KMapSolver.hpp"
#include <iostream>
#include <stdexcept>

KMapSolver::KMapSolver(const std::vector<std::vector<int>>& kmap) : kmap(kmap) {
    numVars = numVariables();
    variables = getVariableNames(numVars);
}

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
    std::cout << "Generated term: " << term << " for rows: ";
    for (const auto& row : rows) {
        std::cout << row << " ";
    }
    std::cout << "and columns: ";
    for (const auto& col : cols) {
        std::cout << col << " ";
    }
    std::cout << std::endl;
    return term;
}

bool KMapSolver::isInGroup(const std::vector<std::pair<int, int>>& group, int r, int c) {
    for (const auto& cell : group) {
        if (cell.first == r && cell.second == c) {
            return true;
        }
    }
    return false;
}

bool KMapSolver::isInside2x2Group(int r, int c) {
    for (const auto& grouping : twoByTwoGroupings) {
        if (isInGroup(grouping.cells, r, c)) {
            return true;
        }
    }
    return false;
}

void KMapSolver::findGroupings(bool isSOP) {
    int rows = kmap.size();
    int cols = kmap[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

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
        std::cout << "Found corner wrap-around grouping at (0,0), (0," << cols - 1 << "), (" << rows - 1 << ",0), (" << rows - 1 << "," << cols - 1 << ")" << std::endl;
    }

    // Find 2x2 groupings
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!visited[r][c] && ((isSOP && kmap[r][c] == 1) || (!isSOP && kmap[r][c] == 0))) {
                if (r + 1 < rows && c + 1 < cols &&
                    kmap[r][c] == kmap[r + 1][c] &&
                    kmap[r][c] == kmap[r][c + 1] &&
                    kmap[r][c] == kmap[r + 1][c + 1]) {
                    Grouping grouping;
                    std::set<int> groupRows = {r, r + 1};
                    std::set<int> groupCols = {c, c + 1};
                    grouping.cells.push_back({r, c});
                    grouping.cells.push_back({r + 1, c});
                    grouping.cells.push_back({r, c + 1});
                    grouping.cells.push_back({r + 1, c + 1});
                    grouping.term = getTerm(groupRows, groupCols, isSOP);
                    if (isSOP) {
                        sopGroupings.push_back(grouping);
                    } else {
                        posGroupings.push_back(grouping);
                    }
                    twoByTwoGroupings.push_back(grouping);
                    visited[r][c] = visited[r + 1][c] = visited[r][c + 1] = visited[r + 1][c + 1] = true;
                    std::cout << "Found 2x2 grouping at (" << r << "," << c << ")" << std::endl;
                }
            }
        }
    }

    // Find 1x2 and 2x1 groupings with cross-referencing against 2x2 groupings
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!visited[r][c] && ((isSOP && kmap[r][c] == 1) || (!isSOP && kmap[r][c] == 0))) {
                if (r + 1 < rows && kmap[r][c] == kmap[r + 1][c] && !isInside2x2Group(r, c) && !isInside2x2Group(r + 1, c)) {
                    Grouping grouping;
                    std::set<int> groupRows = {r, r + 1};
                    std::set<int> groupCols = {c};
                    grouping.cells.push_back({r, c});
                    grouping.cells.push_back({r + 1, c});
                    grouping.term = getTerm(groupRows, groupCols, isSOP);
                    if (isSOP) {
                        sopGroupings.push_back(grouping);
                    } else {
                        posGroupings.push_back(grouping);
                    }
                    visited[r][c] = visited[r + 1][c] = true;
                    std::cout << "Found 1x2 grouping at (" << r << "," << c << ")" << std::endl;
                } else if (c + 1 < cols && kmap[r][c] == kmap[r][c + 1] && !isInside2x2Group(r, c) && !isInside2x2Group(r, c + 1)) {
                    Grouping grouping;
                    std::set<int> groupRows = {r};
                    std::set<int> groupCols = {c, c + 1};
                    grouping.cells.push_back({r, c});
                    grouping.cells.push_back({r, c + 1});
                    grouping.term = getTerm(groupRows, groupCols, isSOP);
                    if (isSOP) {
                        sopGroupings.push_back(grouping);
                    } else {
                        posGroupings.push_back(grouping);
                    }
                    visited[r][c] = visited[r][c + 1] = true;
                    std::cout << "Found 2x1 grouping at (" << r << "," << c << ")" << std::endl;
                }
            }
        }
    }

    // Find single cell groupings with cross-referencing against 2x2 groupings
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
                std::cout << "Found single cell grouping at (" << r << "," << c << ")" << std::endl;

                // Try to group with adjacent cells in 2x2 groups
                if (r + 1 < rows && isInside2x2Group(r + 1, c)) {
                    Grouping &lastGrouping = sopGroupings.back();
                    lastGrouping.cells.push_back({r + 1, c});
                    lastGrouping.term = getTerm({r, r + 1}, {c}, isSOP);
                } else if (r - 1 >= 0 && isInside2x2Group(r - 1, c)) {
                    Grouping &lastGrouping = sopGroupings.back();
                    lastGrouping.cells.push_back({r - 1, c});
                    lastGrouping.term = getTerm({r - 1, r}, {c}, isSOP);
                } else if (c + 1 < cols && isInside2x2Group(r, c + 1)) {
                    Grouping &lastGrouping = sopGroupings.back();
                    lastGrouping.cells.push_back({r, c + 1});
                    lastGrouping.term = getTerm({r}, {c, c + 1}, isSOP);
                } else if (c - 1 >= 0 && isInside2x2Group(r, c - 1)) {
                    Grouping &lastGrouping = sopGroupings.back();
                    lastGrouping.cells.push_back({r, c - 1});
                    lastGrouping.term = getTerm({r}, {c - 1, c}, isSOP);
                }
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
            std::cout << "Found vertical wrap-around grouping at (0," << c << ") and (" << rows - 1 << "," << c << ")" << std::endl;
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
            std::cout << "Found horizontal wrap-around grouping at (" << r << ",0) and (" << r << "," << cols - 1 << ")" << std::endl;
        }
    }
}

std::string KMapSolver::generateExpression(const std::list<Grouping>& groupings, bool isSOP) {
    std::string expression;
    for (const auto& grouping : groupings) {
        if (!expression.empty()) {
            expression += isSOP ? " + " : " ";
        }
        expression += isSOP ? grouping.term : "(" + grouping.term + ")";
    }
    return expression;
}

std::pair<std::string, std::string> KMapSolver::kmapToSOPandPOS() {
    findGroupings(true);  // Find SOP groupings
    findGroupings(false); // Find POS groupings

    std::string sop_result = generateExpression(sopGroupings, true);
    std::string pos_result = generateExpression(posGroupings, false);

    return {sop_result, pos_result};
}

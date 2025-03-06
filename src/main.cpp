#include "KMapSolver.hpp"
#include <iostream>

int main() {
    std::vector<std::vector<int>> kmap = {
        {1, 0, 0, 1},
        {1, 1, 0, 0},
        {0, 1, 1, 1},
        {1, 0, 0, 1}
    };

    KMapSolver solver(kmap);
    auto [sop_result, pos_result] = solver.kmapToSOPandPOS();
    std::cout << "SOP: " << sop_result << std::endl;
    std::cout << "POS: " << pos_result << std::endl;

    return 0;
}

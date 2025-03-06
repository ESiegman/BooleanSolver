#ifndef KMAPSOLVER_HPP
#define KMAPSOLVER_HPP

#include <vector>
#include <string>
#include <list>
#include <set>

struct Grouping {
    std::vector<std::pair<int, int>> cells;
    std::string term;
};

class KMapSolver {
public:
    KMapSolver(const std::vector<std::vector<int>>& kmap);

    std::pair<std::string, std::string> kmapToSOPandPOS();

private:
    std::vector<std::vector<int>> kmap;
    int numVars;
    std::vector<std::string> variables;
    std::list<Grouping> sopGroupings;
    std::list<Grouping> posGroupings;
    std::vector<Grouping> twoByTwoGroupings;

    int numVariables();
    std::vector<std::string> getVariableNames(int num_vars);
    std::string getTerm(const std::set<int>& rows, const std::set<int>& cols, bool isSOP);
    bool isInGroup(const std::vector<std::pair<int, int>>& group, int r, int c);
    bool isInside2x2Group(int r, int c);
    void findGroupings(bool isSOP);
    std::string generateExpression(const std::list<Grouping>& groupings, bool isSOP);
};

#endif // KMAPSOLVER_HPP

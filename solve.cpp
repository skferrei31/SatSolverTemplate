/**
* @author SKF & MAJH
* @date 21.01.26
* @brief
*/

#include "Solver.hpp"
#include "inout.hpp"
#include <iostream>
#include <fstream>


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./solve <input.cnf>" << std::endl;
        return 1;
    }

    // 1. Load Problem
    std::ifstream ifs(argv[1]);
    auto [clauses, numVars] = sat::inout::read_from_dimacs(ifs);

    // 2. Configure Solver
    sat::Solver solver(numVars);
    for (auto& c : clauses) {
        if (!solver.addClause(sat::Clause(std::move(c)))) {
            // If addClause returns false immediately, it's trivial UNSAT
            std::cout << "UNSAT" << std::endl;
            return 0;
        }
    }

    // 3. Run Search
    if (solver.solve()) {
        // 4. Print Result
        std::cout << "s SATISFIABLE" << std::endl; // Optional status line
        // Use the provided to_dimacs helper for the literals
        sat::inout::to_dimacs(std::cout, solver.getUnitLiterals()); 
    } else {
        std::cout << "UNSAT" << std::endl;
    }

    return 0;
}
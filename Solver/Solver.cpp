/**
* @author Tim Luchterhand
* @date 27.11.24
* @brief
*/

#include "Solver.hpp"
#include "util/exception.hpp"

namespace sat {
    Solver::Solver(unsigned numVariables) {
        this->model.assign(numVariables, TruthValue::Undefined);
    }

    bool Solver::addClause(Clause clause) {
        if (clause.size() > 1) {
            //create pointer for the clause we want to add
            ClausePointer ptr = std::make_shared<Clause>(std::move(clause));
            
            this->clauses.push_back(ptr);
            return true; 
        } else { //cannot add an empty or unit clause
            return false;
        }
    }

    /**
     * Here you have a possible implementation of the rebase-method. It should work out of the box.
     * To use it, remove the throw-expression and un-comment the code below. The implementation requires that
     * your solver has some sort of container of pointer types to clause called 'clauses'
     * (e.g. std::vector<ClausePointer>). Additionally, your solver needs to have a container of unit literals
     * called 'unitLiterals'.
     */
    auto Solver::rebase() const -> std::vector<Clause> {

        std::vector<Clause> reducedClauses;

        // We check all clauses in the solver. If the clause is SAT (at least one literal is satisfied), we don't include it.
        // Additionally, we remove all falsified literals from the clauses since we only care about unassigned literals.
        for (const auto &c: clauses) {
            bool sat = false;
            // We're creating a new (possibly smaller clause from the current clause c)
            std::vector<Literal> newLits;
            newLits.reserve(c->size());
            // For each literal l in the clause we check if l is satisfied or falsified
            for (auto l: *c) {
                if (satisfied(l)) {
                    // in this case, the whole clause is satisfied and can be removed entirely from the solver
                    sat = true;
                    break;
                }

                if (!falsified(l)) {
                    // only unassigned literals are added to the reduced clause
                    newLits.emplace_back(l);
                }
            }

            if (!sat) {
                // create the new clause (move all the literals inside the Clause-class)
                Clause newClause(std::move(newLits));
                // Check if we already added an equivalent clause
                auto res = std::ranges::find_if(reducedClauses, [&newClause](const auto &clause) {
                    return clause.sameLiterals(newClause);
                });
                if (res == reducedClauses.end()) {
                    // The new clause is not yet contained => add it
                    reducedClauses.emplace_back(std::move(newClause));
                }
            }
        }

        // Finally, we need to add all the unit literals as well
        for (Literal l: unitLiterals) {
            reducedClauses.emplace_back(std::vector{l});
        }

        return reducedClauses;
    }

    TruthValue Solver::val(Variable x) const {
        //if variable is not in model then it's undefined
        if(x.get() >= this->model.size()) {
            return TruthValue::Undefined;
        } else { //else we just check in the model
            return this->model[x.get()];
        }
    }

    bool Solver::satisfied(Literal l) const {
        Variable l_var = var(l);

        TruthValue l_value = this->model[l_var.get()];

        //if varibale undefined then it's satisfied
        if (l_value == TruthValue::Undefined) {
            return false;
        }

        //if literal is positive and value is true in the model, or literal is negative and false in the model
        if ((l_value == TruthValue::True && l.sign() == 1) || (l_value == TruthValue::False && l.sign() == -1 )) {
            return true;
        } else {
            return false;
        }
    }

    bool Solver::falsified(Literal l) const {
        /*
        Variable l_var = var(l);

        TruthValue l_value = this->model[l_var.get()];

        //if varibale undefined then it's satisfied
        if (l_value == TruthValue::Undefined) {
            return false;
        }

        //if literal is positive and value is false in the model, or literal is negative and true in the model
        if (l_value == TruthValue::True && l.sign() == -1 || l_value == TruthValue::False && l.sign() == 1 ) {
            return true;
        } else {
            return false;
        }
        */

        Variable x = var(l);
        TruthValue see = this->model[x.get()];
        if (see == TruthValue::Undefined) return false;

        if (see == TruthValue::True) return l.sign() == -1;
        if (see == TruthValue::False) return l.sign() == 1;
        else return false;
    }

    bool Solver::assign(Literal l) {
        Variable l_var = var(l);
        TruthValue l_value = this->model[l_var.get()];

        //if not defined, then define
        if (l_value == TruthValue::Undefined) {
            if (l.sign() == 1) {
                this->model[l_var.get()] = TruthValue::True;
            } else { //l.sign() == -1
                this->model[l_var.get()] = TruthValue::False;
            }

            //if we define a literal, we add it in the unitLiterals list
            this->unitLiterals.push_back(l);
            return true;
        } 

        //if already defined, if current value is same as new value, then assignment OK (nothing changes)
        if ((l_value == TruthValue::True && l.sign() == 1) || (l_value == TruthValue::False && l.sign() == -1 )) {
            //l is supposed to be in unitLiterals already so we don't add it in again
            return true;
        } 
        
        //else value can't change anyway so assignment KO
        return false;
    }

    bool Solver::unitPropagate() {
        throw NOT_IMPLEMENTED;

        //we search a unit clause
            //if there is: we take the clause and assign the literal l in the model as true to its correspodning variable
            //if there isn't: we take the first clause, and assign the first literal l as true arbitrarly

        //then, we propagate:
            //each clause contaning l is removed from the solver 
            //each clause contaning not_l is reduced 
            //+++protip: normally rebase() is used for this

        //then we repeat steps 1 and 2 until every variable in the model is assigned
            //if the model is true, then we finished
            //if the model is false, we go back to the point where we made the most recent arbitrary assignment, make another choice then repeat
                //if both choices failed, we go back to the previous arbitrary assignment and mak another choice
                    /*if there are no more possibilities of arbitrary assignments to be made and the model is still false, 
                    then it's over and the model is unsolvable */
        
        
    }
} // sat
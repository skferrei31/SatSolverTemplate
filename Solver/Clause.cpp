/**
* @author Tim Luchterhand
* @date 26.11.24
* @brief
*/

#include <cassert>
#include <algorithm>

#include "Clause.hpp"
#include "util/exception.hpp"

namespace sat {
    //TODO implementation here

    Clause::Clause(std::vector<Literal> literals) {
        this->literals = literals;
    }

    short Clause::getRank(Literal l) const {
        if (this->literals[firstWatcher] == l) {
            return 0;
        } else if (this->literals[secondWatcher] == l) {
            return 1;
        } else { //not a watcher
            return -1;
        }
    }

    std::size_t Clause::getIndex(short rank) const {
        if (rank == 0) {
            return this->firstWatcher;
        } else { //rank == 1 OR rank == -1
            return this->secondWatcher;
        }
    }

    bool Clause::setWatcher(Literal l, short watcherNo) {
        // std::find returns an "iterator" (like a pointer) to the element
        auto it = std::find(literals.begin(), literals.end(), l);

        // If it equals literals.end(), it means it searched the whole thing and found nothing
        if (it == literals.end()) {
            return false;
        }

        // To turn an iterator back into a number (index), we subtract the beginning
        std::size_t index = std::distance(literals.begin(), it);

        if (watcherNo == 0) {
            this->firstWatcher = index;
        } else {
            this->secondWatcher = index;
        }

        return true;
    }

    auto Clause::begin() const -> std::vector<Literal>::const_iterator {
        // Returns an iterator to the first literal
        return literals.begin();
    }

    auto Clause::end() const -> std::vector<Literal>::const_iterator {
        // Returns an iterator to the "past-the-end" position
        return literals.end();
    }

    bool Clause::isEmpty() const {
        return this->literals.size() == 0;
    }

    Literal Clause::operator[](std::size_t index) const {
        return this->literals[index];
    }

    std::size_t Clause::size() const {
        return this->literals.size();
    }

    Literal Clause::getWatcherByRank(short rank) const {
        if (rank == 0) {
            return this->literals[firstWatcher];
        } else { //rank == 1 OR rank == -1
            return this->literals[secondWatcher];
        } 
    }

    bool Clause::sameLiterals(const Clause &other) const {
        //if clauses are not the same size, they wont have the same literals anyway
        if (this->size() != other.size()) {
            return false;
        }

        //make copies so we don't mess up the original order of the clauses
        std::vector<Literal> copy1 = this->literals;
        std::vector<Literal> copy2 = other.literals;

        //define rule used to tell how to compare Literals
        auto comp = [](Literal a, Literal b) {
            return a.get() < b.get();
        };

        //sort both copies using our rule
        std::sort(copy1.begin(), copy1.end(), comp);
        std::sort(copy2.begin(), copy2.end(), comp);
        
        /* method with projection
        is supposed to work something's missing here apparently
        std::sort(copy1.begin(), copy1.end(), {}, [](Literal l) {return l.get();});
        std::sort(copy2.begin(), copy2.end(), {}, [](Literal l) {return l.get();});
        */

        //compare the sorted vectors
        //this works now because Literals are in a predictable order
        //actually a hidden loop, literals are comapred one by one using the == we defined in basic_structures for Literal type
        return copy1 == copy2;
    }

}

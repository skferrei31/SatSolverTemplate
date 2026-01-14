/**
* @author Tim Luchterhand
* @date 26.11.24
* @brief
*/

#include "basic_structures.hpp"
#include "util/exception.hpp"

namespace sat {
    // @TODO implementation here


    Variable::Variable(unsigned val) {
        this->id = val;
    }

    unsigned Variable::get() const {
        return id;
    }

    bool Variable::operator==(Variable other) const {
        return this->id == other.id;
    }

    Literal::Literal(unsigned val) {
        this->id = val;
    }

    unsigned Literal::get() const {
        return id;
    }

    Literal Literal::negate() const {
        if (this->id % 2 == 0) {
            return Literal(this->id + 1);
        } else { //this->id % 2 == 1
            return Literal(this->id - 1);
        }
    }

    short Literal::sign() const {
        if (this->id % 2 == 0) {
            return -1;
        } else { //this->id % 2 == 1
            return 1;
        }
    }

    bool Literal::operator==(Literal other) const {
        return (this->id / 2 == other.id / 2) && (this->sign() == other.sign());
    }

    Literal pos(Variable x) {
        return Literal(2*x.get()+1);
    }

    Literal neg(Variable x) {
        return Literal(2*x.get());
    }

    Variable var(Literal l) {
        if (l.sign() == 1) {
            return Variable((l.get()-1)/2);
        } else { //l.sign() == -1
            return Variable(l.get()/2);
        }
    }
}

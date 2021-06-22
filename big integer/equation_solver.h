//
// Created by Computer on 29.10.2019.
//

#ifndef EQUATION_SOLVER_H_
#define EQUATION_SOLVER_H_

#include "big_integer.h"

using big_num_arithmetic::BigInteger;

namespace equation_solver {

struct QuadraticEquation {
    BigInteger a;
    BigInteger b;
    BigInteger c;
};

bool Solve(const QuadraticEquation &equation,
            BigInteger &x1,
            BigInteger &x2);

namespace helpers {
    BigInteger Sqrt(const BigInteger &val);
}  // namespace helpers

namespace big_num_arithmetic {
    QuadraticEquation
        GenerateEquation(const BigInteger& a,
                         const BigInteger& x1,
                         const BigInteger& x2);
}  // namespace big_num_arithmetic

}  // namespace equation_solver

#endif  // EQUATION_SOLVER_H_

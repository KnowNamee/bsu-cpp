//
// Created by Computer on 29.10.2019.
//

#include <stdexcept>
#include "equation_solver.h"
#include "big_integer.h"

using big_num_arithmetic::BigInteger;

namespace equation_solver {

bool Solve(const QuadraticEquation &equation,
                BigInteger &x1,
                BigInteger &x2) {
    const BigInteger &a = equation.a,
                     &c = equation.c,
                     &b = equation.b;
    BigInteger D = (b * b) - (a * 4);

    if (D < 0) {
        return false;
    }

    BigInteger sqrt_d = helpers::Sqrt(D),
               mins_b = b * (-1),
               two_a  = a * 2;

    BigInteger x1_cur = (mins_b + sqrt_d) / two_a,
               x2_cur = (mins_b - sqrt_d) / two_a;

    return (x1 == x1_cur && x2 == x2_cur) ||
           (x1 == x2_cur && x2 == x1_cur);
}

namespace helpers {

BigInteger Sqrt(const BigInteger &val) {
    if (val < 0) {
        throw std::runtime_error("Bad value");
    }

    BigInteger r(val);
    BigInteger l;
    BigInteger x;

    while (l <= r) {
        BigInteger m = (l + r) / 2;
        x = m * m;

        if (x == val) {
            return m;
        } else if (x < val) {
            l = m + 1;
        } else {
            r = m - 1;
        }
    }

    return BigInteger(0);
}

}  // namespace helpers

namespace big_num_arithmetic {

QuadraticEquation
GenerateEquation(const BigInteger &a,
                 const BigInteger &x1,
                 const BigInteger &x2) {
    BigInteger b = a * (x1 + x2) * (-1),
               c = a * x1 * x2;
    return {a, b, c};
}

}  // namespace big_num_arithmetic

}  // namespace equation_solver


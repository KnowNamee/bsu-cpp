//
// Created by ZenBook on 05.11.2019.
//

#include "equation_solver.h"
#include <gtest.h>
#include <random>
#include "big_integer.h"

using big_num_arithmetic::BigInteger;

BigInteger GenBInteger() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint32_t> dist(INT32_MIN, INT32_MAX);
    int64_t num = 0;
    while (!num) {
        num = dist(mt);
    }
    return BigInteger(num);
}

namespace equation_solver {

    TEST(EqSolverTests, Sqrt) {
        for (int i = 2; i <= 10000; i++) {
            BigInteger::SetInternalBase(i);
            for (int j = 0; j < 100; j++) {
                EXPECT_EQ(helpers::Sqrt(BigInteger(i*i)), i);
            }
        }
    }

    TEST(EqSolverTests, QuadraticEquations) {
        for (int x = 2; x <= 10; x++) {
            std::cout << "start " << x << std::endl;
            int q = 1'000;
            while (q--) {
                BigInteger a(GenBInteger()),
                        x1(GenBInteger()),
                        x2(GenBInteger());
                QuadraticEquation eq =
                        big_num_arithmetic::GenerateEquation(a, x1, x2);

                EXPECT_TRUE(Solve(eq, x1, x2));
            }
        }
    }

}  // namespace equation_solver


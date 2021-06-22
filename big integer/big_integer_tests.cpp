//
// Created by ZenBook on 04.11.2019.
//

#include <random>
#include "big_integer.h"
#include "gtest.h"

std::vector<int64_t> GenData(int size) {
    std::vector<int64_t> data(size);
    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution<int64_t> dist(INT32_MIN, INT32_MAX);

    for (auto &x : data) {
        x = dist(mt);
    }
    data.push_back(0);

    return data;
}

namespace big_num_arithmetic {
    int Sign(int64_t v) {
        return (v > 0 ? 1 : v < 0 ? -1 : 0);
    }

    TEST(BigIntegerTests, Sign) {
        std::vector<int64_t> v = GenData(1'000);

        for (auto x : v) {
            if (x > 0) {
                EXPECT_EQ(BigInteger(x).Sign(), 1);
            } else if (x < 0) {
                EXPECT_EQ(BigInteger(x).Sign(), -1);
            } else {
                EXPECT_EQ(BigInteger(x).Sign(), 0);
            }
        }
    }

    TEST(BigIntegerTests, Abs) {
        std::vector<int64_t> v = GenData(1'000);

        for (auto x : v) {
            BigInteger val = BigInteger(x);
            val.Abs();
            if (x != 0) {
                EXPECT_EQ(val.Sign(), 1);
            } else {
                EXPECT_EQ(val.Sign(), 0);
            }
        }
    }

    TEST(BigIntegerTests, Negate) {
        std::vector<int64_t> v = GenData(1'000);

        for (auto x : v) {
            BigInteger val(x);
            val.Negate();
            if (x > 0) {
                EXPECT_EQ(val.Sign(), -1);
            } else if (x < 0) {
                EXPECT_EQ(val.Sign(), 1);
            } else {
                EXPECT_EQ(val.Sign(), 0);
            }
        }
    }

    TEST(BigIntegerTests, ComparisonOperators) {
        std::vector<int64_t> v = GenData(1'000);

        for (auto x  : v) {
            for (auto y : v) {
                BigInteger bx(x);
                BigInteger by(y);

                EXPECT_EQ(bx == by, x == y);
                EXPECT_EQ(bx != by, x != y);
                EXPECT_EQ(bx >= by, x >= y);
                EXPECT_EQ(bx <= by, x <= y);
                EXPECT_EQ(bx > by, x > y);
                EXPECT_EQ(bx < by, x < y);
            }
        }
    }

    TEST(BigIntegerTests, BinaryOperators) {
        std::vector<int64_t> v = GenData(1'000);

        for (auto x : v) {
            for (auto y : v) {
                BigInteger bx(x), by(y);

                BigInteger b_plus = bx + by;
                int64_t c_plus = x + y;
                EXPECT_EQ(b_plus, c_plus);
                EXPECT_EQ(b_plus.Sign(), Sign(c_plus));

                BigInteger b_minus = bx - by;
                int64_t c_minus = x - y;
                EXPECT_EQ(b_minus, c_minus);
                EXPECT_EQ(b_minus.Sign(), Sign(c_minus));

                BigInteger b_mult = bx * by;
                int64_t c_mult = x * y;
                EXPECT_EQ(b_mult, c_mult);
                EXPECT_EQ(b_mult.Sign(), Sign(c_mult));

                if (!y) {
                    std::swap(x, y);
                    std::swap(bx, by);
                }
                BigInteger b_div = bx / by;
                int64_t c_div = x / y;
                EXPECT_EQ(b_div, c_div);
                EXPECT_EQ(b_div.Sign(), Sign(c_div));
            }
        }
    }

    TEST(BigIntegerTests, Modulo) {
        std::vector<int64_t> v = {17, -1, 25, 11, 12, 23, 42, 2, -23424};

        for (auto x : v) {
            x = std::abs(x);
            for (auto y : v) {
                y = std::abs(y);
                BigInteger bx(x);

                EXPECT_EQ(bx % y, x % y);
            }
        }
    }

    TEST(BigIntegerTests, FromString) {
        BigInteger bi = BigInteger::FromString("-0000009", 11);

        EXPECT_EQ(bi, -9);

        bi = BigInteger::FromString("-0000000000900001", 10);

        EXPECT_EQ(bi, -900001);
    }

    TEST(BigIntegerTests, ToString) {
        BigInteger::SetInternalBase(2);
        BigInteger bi = BigInteger::FromString("-1111111", 2);
        std::string str = bi.ToString(3);
        std::cout << str << std::endl;
    }

    TEST(BigIntegerTests, UnaryOperators) {
        std::vector<int64_t> v = GenData(1000);

        for (auto x : v) {
            BigInteger bx(x);

            ++bx;
            ++x;
            EXPECT_EQ(bx, x);
            bx++;
            x++;
            EXPECT_EQ(bx, x);
            --bx, --x;
            EXPECT_EQ(bx, x);
            bx--, x--;
            EXPECT_EQ(bx, x);
        }
    }
}  // namespace big_num_arithmetic

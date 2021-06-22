//
// Created by Computer on 28.10.2019.
//

#ifndef BIG_INTEGER_H_
#define BIG_INTEGER_H_

#include <cstdint>
#include <cmath>
#include <vector>
#include <ostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <map>
#include <stdexcept>

namespace big_num_arithmetic {

struct DivisionByZeroError : std::exception {};

class BigInteger {
 public:
    static int internal_base;

    BigInteger();
    BigInteger(const BigInteger &other);
    explicit BigInteger(int64_t integer);

    int Sign() const;
    void Abs();
    void Negate();

    static void SetInternalBase(int);

    static BigInteger FromString(const std::string &, int base);
    std::string ToString(int base) const;

    // Compare operators
    bool operator==(const BigInteger &) const;
    bool operator!=(const BigInteger &) const;
    bool operator> (const BigInteger &) const;
    bool operator< (const BigInteger &) const;
    bool operator>=(const BigInteger &) const;
    bool operator<=(const BigInteger &) const;

    bool operator==(int64_t) const;
    bool operator!=(int64_t) const;
    bool operator> (int64_t) const;
    bool operator< (int64_t) const;
    bool operator>=(int64_t) const;
    bool operator<=(int64_t) const;

    friend bool operator==(int64_t, const BigInteger &n);
    friend bool operator!=(int64_t, const BigInteger &n);
    friend bool operator> (int64_t, const BigInteger &n);
    friend bool operator< (int64_t, const BigInteger &n);
    friend bool operator>=(int64_t, const BigInteger &n);
    friend bool operator<=(int64_t, const BigInteger &n);

    // Arithmetic operators
    BigInteger &operator+=(const BigInteger &);
    BigInteger &operator-=(const BigInteger &);
    BigInteger &operator*=(const BigInteger &);
    BigInteger &operator/=(const BigInteger &);

    BigInteger &operator+=(int64_t);
    BigInteger &operator-=(int64_t);
    BigInteger &operator*=(int64_t);
    BigInteger &operator/=(int64_t);

    BigInteger operator+(const BigInteger &) const;
    BigInteger operator-(const BigInteger &) const;
    BigInteger operator*(const BigInteger &) const;
    BigInteger operator/(const BigInteger &) const;

    BigInteger operator+(int64_t) const;
    BigInteger operator-(int64_t) const;
    BigInteger operator*(int64_t) const;
    BigInteger operator/(int64_t) const;

    friend BigInteger operator+(int64_t, const BigInteger &);
    friend BigInteger operator-(int64_t, const BigInteger &);
    friend BigInteger operator*(int64_t, const BigInteger &);
    friend BigInteger operator/(int64_t, const BigInteger &);

    BigInteger &operator++();
    BigInteger operator++(int);
    BigInteger &operator--();
    BigInteger operator--(int);

    uint32_t operator%(uint32_t) const;

    explicit operator int64_t() const;

 private:
    std::vector<int64_t> number;
    int sign;

    void RemoveLeadingNulls();
    void ExpandNumber();
    void ChangeSignIfNeeded();

    static int GetIntValue(int, int, int);
    static char GetCharValue(int);
};

}  // namespace big_num_arithmetic

#endif  // BIG_INTEGER_H_

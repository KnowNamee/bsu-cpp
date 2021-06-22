//
// Created by Computer on 28.10.2019.
//

#include <stdexcept>
#include "big_integer.h"

namespace big_num_arithmetic {

    int BigInteger::internal_base = 10'000;

    BigInteger::BigInteger() {
        sign = 0;
        number.push_back(0);
    }

    BigInteger::BigInteger(int64_t integer) {
        this->number.clear();
        this->sign = (integer < 0 ? -1 : integer == 0 ? 0 : 1);

        do {
            this->number.push_back(std::abs(integer % internal_base));
            integer /= internal_base;
        } while (integer != 0);
    }

    BigInteger::BigInteger(const BigInteger &other) {
        this->sign = other.sign;
        this->number = other.number;
    }

    // Public methods
    int BigInteger::Sign() const {
        return (number.empty() ? 0 : sign);
    }

    void BigInteger::Abs() {
        sign *= sign;
    }

    void BigInteger::Negate() {
        sign = -sign;
    }

    BigInteger BigInteger::FromString(const std::string &str, int base) {
        bool sign = (str[0] == '-');
        int len = str.length();
        BigInteger val;
        BigInteger base_power(1);

        for (int i = sign; i < len; i++) {
            GetIntValue(str[i], base, i);
        }

        for (int i = len - 1; i > 0 || (i == 0 && !sign); i--) {
            val += base_power * GetIntValue(str[i], base, i);
            base_power *= base;
        }

        if (val != 0) {
            val.sign = (sign ? -1 : 1);
        }
        return val;
    }

    std::string BigInteger::ToString(int base) const {
        if (base < 2 || base > 36) {
            throw std::logic_error("Invalid base");
        }

        std::string ans = "";
        BigInteger obj(*this);
        obj.Abs();

        while (obj > 0) {
               ans += GetCharValue(obj % base);
               obj /= base;
        }

        if (*this < 0) {
            ans += "-";
        }
        std::reverse(ans.begin(), ans.end());
        return (ans == "" ? "0" : ans);
    }

    // Private methods

    void BigInteger::RemoveLeadingNulls() {
        while (!number.empty() && number.back() == 0) {
            number.pop_back();
        }

        if (number.empty()) {
            *this = BigInteger();
        }
    }

    void BigInteger::ExpandNumber() {
        int sz = number.size();
        for (int i = 0; i < sz; i++) {
            if (number[i] >= internal_base) {
                int64_t add_to_next = number[i] / internal_base;
                number[i] %= internal_base;
                if (i + 1 == sz) {
                    number.push_back(0);
                    sz++;
                }
                if (i + 1 < sz) {
                    number[i + 1] += add_to_next;
                }
            }
        }
    }

    void BigInteger::ChangeSignIfNeeded() {
        int nb = number.back();
        int ns = number.size();

        if (nb > 0) {
            sign = 1;
        } else if (nb < 0) {
            sign = -1;
            for (int64_t &x : number) {
                x = -x;
            }
        } else {
            return;
        }

        for (int i = 0; i < ns; i++) {
            if (number[i] < 0) {
                number[i + 1]--;
                number[i] += BigInteger::internal_base;
            }
        }

        this->RemoveLeadingNulls();
    }

    int BigInteger::GetIntValue(int c, int base, int i) {
        if ('0' <= c && c <= '0' + std::min(9, base - 1)) {
            return c - '0';
        } else if ('a' <= c &&
                    c <= 'a' + std::max(-1, std::min(36, base) - 11)) {
            return c - 'a' + 10;
        } else {
            throw std::runtime_error("Invalid symbol at index "
                                                  + std::to_string(i));
        }
    }

    char BigInteger::GetCharValue(int ost) {
        if (0 <= ost && ost < 10) {
            return '0' + ost;
        } else {
            return 'a' + ost - 10;
        }
    }

    void BigInteger::SetInternalBase(int base) {
        BigInteger::internal_base = base;
    }

    // Compare operators

    bool BigInteger::operator==(const BigInteger &rhs) const {
        if (number.size() != rhs.number.size() || sign != rhs.sign) {
            return false;
        }

        int szl = number.size();
        for (int i = 0; i < szl; i++) {
            if (number[i] != rhs.number[i]) {
                return false;
            }
        }

        return true;
    }

    bool BigInteger::operator>(const BigInteger &rhs) const {
        if (sign != rhs.sign) {
            return sign > rhs.sign;
        } else {
            int szl = number.size();
            int szr = rhs.number.size();

            if (szl != szr) {
                return szl * sign > szr * rhs.sign;
            }

            for (int i = szl - 1; i >= 0; i--) {
                if (number[i] == rhs.number[i]) {
                    continue;
                }
                return number[i] * sign > rhs.number[i] * rhs.sign;
            }
        }
        return false;
    }

    bool BigInteger::operator<(const BigInteger &rhs) const {
        if (sign != rhs.sign) {
            return sign < rhs.sign;
        } else {
            int szl = number.size();
            int szr = rhs.number.size();

            if (szl != szr) {
                return szl * sign < szr * rhs.sign;
            }

            for (int i = szl - 1; i >= 0; i--) {
                if (number[i] == rhs.number[i]) {
                    continue;
                }
                return number[i] * sign < rhs.number[i] * rhs.sign;
            }
        }
        return false;
    }

    bool BigInteger::operator!=(const BigInteger &rhs) const {
        return !(*this == rhs);
    }

    bool BigInteger::operator>=(const BigInteger &rhs) const {
        return !(*this < rhs);
    }

    bool BigInteger::operator<=(const BigInteger &rhs) const {
        return !(*this > rhs);
    }

    bool BigInteger::operator==(int64_t rhs) const {
        return *this == BigInteger(rhs);
    }

    bool BigInteger::operator!=(int64_t rhs) const {
        return *this != BigInteger(rhs);
    }

    bool BigInteger::operator<(int64_t rhs) const {
        return *this < BigInteger(rhs);
    }

    bool BigInteger::operator>(int64_t rhs) const {
        return *this > BigInteger(rhs);
    }

    bool BigInteger::operator>=(int64_t rhs) const {
        return *this >= BigInteger(rhs);
    }

    bool BigInteger::operator<=(int64_t rhs) const {
        return *this <= BigInteger(rhs);
    }

    bool operator==(int64_t lhs, const BigInteger& rhs) {
        return BigInteger(lhs) == rhs;
    }

    bool operator!=(int64_t lhs, const BigInteger& rhs) {
        return BigInteger(lhs) != rhs;
    }

    bool operator<(int64_t lhs, const BigInteger& rhs) {
        return BigInteger(lhs) < rhs;
    }

    bool operator>(int64_t lhs, const BigInteger& rhs) {
        return BigInteger(lhs) > rhs;
    }

    bool operator<=(int64_t lhs, const BigInteger& rhs) {
        return BigInteger(lhs) <= rhs;
    }

    bool operator>=(int64_t lhs, const BigInteger& rhs) {
        return BigInteger(lhs) >= rhs;
    }

    // Arithmetic operators

    BigInteger& BigInteger::operator/=(const BigInteger& rhs) {
        if (rhs == 0) {
            throw DivisionByZeroError();
        }

        BigInteger right = rhs;
        int rsgn = right.sign;
        int lsgn = this->sign;

        if (rsgn == -1) {
            right.Negate();
        }
        if (lsgn == -1) {
            this->Negate();
        }

        BigInteger x;
        BigInteger l;
        BigInteger r(*this);

        while (l <= r) {
            BigInteger m = (l + r) / 2;
            BigInteger buf = right * m;

            if (buf <= *this) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }

        x.sign = rsgn * lsgn * x.sign;
        return (*this = x);
    }

    BigInteger& BigInteger::operator+=(const BigInteger &rhs) {
        number.resize(std::max(rhs.number.size(), this->number.size()));

        int i = 0;
        int lsz = rhs.number.size();
        int rsz = this->number.size();
        while (i < lsz) {
            number[i] = (number[i] * sign) + (rhs.number[i] * rhs.sign);
            i++;
        }
        while (i < rsz) {
            number[i] *= sign;
            i++;
        }

        this->RemoveLeadingNulls();
        this->ChangeSignIfNeeded();
        this->ExpandNumber();
        this->RemoveLeadingNulls();

        return *this;
    }

    BigInteger& BigInteger::operator*=(const BigInteger &lhs) {
        BigInteger ans;
        int rsz = this->number.size();
        int lsz = lhs.number.size();

        ans.sign = sign * lhs.sign;
        ans.number.resize(rsz + lsz, 0);

        for (int i = 0; i < rsz; i++) {
            for (int j = 0; j < lsz; j++) {
                ans.number[i + j] += number[i] * lhs.number[j];
            }
            ans.ExpandNumber();
        }
        ans.RemoveLeadingNulls();

        return (*this = ans);
    }

    BigInteger& BigInteger::operator-=(const BigInteger &rhs) {
        BigInteger r_int(rhs); r_int.Negate();
        return *this += r_int;
    }

    BigInteger BigInteger::operator+(const BigInteger &rhs) const {
        return BigInteger(*this) += rhs;
    }

    BigInteger BigInteger::operator-(const BigInteger &rhs) const {
        return BigInteger(*this) -= rhs;
    }

    BigInteger BigInteger::operator/(const BigInteger &rhs) const {
        return BigInteger(*this) /= rhs;
    }

    BigInteger BigInteger::operator*(const BigInteger &rhs) const {
        return BigInteger(*this) *= rhs;
    }

    BigInteger BigInteger::operator/(int64_t rhs) const {
        BigInteger res = *this;
        if (rhs == 2) {
            int sz = res.number.size();
            for (int i = sz - 1; i >= 0; i--) {
                int ost = res.number[i] % 2;
                res.number[i] /= 2;
                if (i - 1 >= 0) {
                    res.number[i - 1] += ost * internal_base;
                }
            }
        } else {
            res /= rhs;
        }
        res.RemoveLeadingNulls();

        return res;
    }

    BigInteger BigInteger::operator*(int64_t rhs) const {
        return BigInteger(*this) *= rhs;
    }

    BigInteger BigInteger::operator+(int64_t rhs) const {
        return BigInteger(*this) += rhs;
    }

    BigInteger BigInteger::operator-(int64_t rhs) const {
        return BigInteger(*this) -= rhs;
    }

    BigInteger& BigInteger::operator-=(int64_t rhs) {
        return *this -= BigInteger(rhs);
    }

    BigInteger& BigInteger::operator+=(int64_t rhs) {
        return *this += BigInteger(rhs);
    }

    BigInteger& BigInteger::operator*=(int64_t rhs) {
        return *this *= BigInteger(rhs);
    }

    BigInteger& BigInteger::operator/=(int64_t rhs) {
        return *this /= BigInteger(rhs);
    }

    BigInteger operator/(int64_t lhs, const BigInteger &rhs) {
        return BigInteger(lhs) /= rhs;
    }

    BigInteger operator*(int64_t lhs, const BigInteger &rhs) {
        return BigInteger(lhs) *= rhs;
    }

    BigInteger operator+(int64_t lhs, const BigInteger &rhs) {
        return BigInteger(lhs) += rhs;
    }

    BigInteger operator-(int64_t lhs, const BigInteger &rhs) {
        BigInteger int_l(lhs);
        BigInteger int_r(rhs);
        int_r.Negate();
        return int_l += int_r;
    }

    BigInteger BigInteger::operator++(int) {
        BigInteger integer(*this);
        *this += 1;
        return integer;
    }

    BigInteger BigInteger::operator--(int) {
        BigInteger integer(*this);
        *this -= 1;
        return integer;
    }

    BigInteger& BigInteger::operator++() {
        return (*this += 1);
    }

    BigInteger& BigInteger::operator--() {
        return (*this -= 1);
    }

    uint32_t BigInteger::operator%(uint32_t rhs) const {
        if (!rhs) {
            throw DivisionByZeroError{};
        }
        int64_t lhs = int64_t(*this - (*this / rhs) * rhs);
        return (lhs < 0 ? lhs + rhs : lhs);
    }

    BigInteger::operator int64_t() const {
        if (*this > std::numeric_limits<int64_t>::max() ||
            *this < std::numeric_limits<int64_t>::min()) {
            throw std::runtime_error("int64_t overflow");
        }

        int64_t val = 0;
        int sz = this->number.size();

        for (int i = sz - 1; i >= 0; i--) {
            val *= BigInteger::internal_base;
            val += (this->number[i]);
        }

        return val * this->sign;
    }

}  // namespace big_num_arithmetic

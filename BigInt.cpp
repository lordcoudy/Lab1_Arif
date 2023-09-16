//
// Created by Milord on 9/14/23.
//

#include "BigInt.h"

BigInt::BigInt()
{
    this->neg = false;
}

BigInt::BigInt(std::string str)
{
    if (str.length() == 0) {
        this->neg = false;
    }
    else if (str.length() > 18)
    {
        throw BigInt::overflow();
    }
    else {
        if (str[0] == '-') {
            str = str.substr(1);
            this->neg = true;
        }
        else {
            this->neg = false;
        }

        for (ll i = static_cast<ll> (str.length()); i > 0; i -= 9) {
            if (i < 9)
                this->digits.push_back(atoi(str.substr(0, i).c_str()));
            else
                this->digits.push_back(atoi(str.substr(i - 9, 9).c_str()));
        }

        this->removeZeros();
    }
}

BigInt::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    if (ss.str().length() > 18)
    {
        throw BigInt::overflow();
    }
    return ss.str();
}

BigInt::BigInt(signed char c) {
    if (c < 0) this->neg = true;
    else this->neg = false;
    this->digits.push_back(std::abs(c));
}

BigInt::BigInt(unsigned char c) {
    this->neg = false;
    this->digits.push_back(c);
}

BigInt::BigInt(signed short s) {
    if (s < 0) this->neg = true;
    else this->neg = false;
    this->digits.push_back(std::abs(s));
}

BigInt::BigInt(unsigned short s) {
    this->neg = false;
    this->digits.push_back(s);
}

BigInt::BigInt(signed int i) {
    if (i < 0) this->neg = true;
    else this->neg = false;
    this->digits.push_back(std::abs(i) % BigInt::base);
    i /= BigInt::base;
    if (i != 0) this->digits.push_back(std::abs(i));
}

BigInt::BigInt(unsigned int i) {
    neg = false;
    this->digits.push_back(i % BigInt::base);
    i /= BigInt::base;
    if (i != 0) this->digits.push_back(i);
}

BigInt::BigInt(signed long l) {
    if (l < 0) this->neg = true;
    else this->neg = false;
    this->digits.push_back(std::abs(l) % BigInt::base);
    l /= BigInt::base;
    if (l != 0) this->digits.push_back(std::abs(l));
}

BigInt::BigInt(unsigned long l) {
    neg = false;
    this->digits.push_back(l % BigInt::base);
    l /= BigInt::base;
    if (l != 0) this->digits.push_back(l);
}

BigInt::BigInt(signed long long l) {
    if (l < 0) { this->neg = true; l = -l; }
    else this->neg = false;
    do {
        this->digits.push_back(l % BigInt::base);
        l /= BigInt::base;
    } while (l != 0);
}

BigInt::BigInt(unsigned long long l) {
    this->neg = false;
    do {
        this->digits.push_back(l % BigInt::base);
        l /= BigInt::base;
    } while (l != 0);
}

void BigInt::removeZeros() {
    while (this->digits.size() > 1 && this->digits.back() == 0) {
        this->digits.pop_back();
    }

    if (this->digits.size() == 1 && this->digits[0] == 0) this->neg = false;
}

std::ostream& operator <<(std::ostream& os, const BigInt& number) {
    if (number.digits.empty()) os << 0;
    else {
        if (number.neg) os << '-';
        os << number.digits.back();
        char old_fill = os.fill('0');
        for (long long i = static_cast<long long>(number.digits.size()) - 2; i >= 0; --i) os << std::setw(9) << number.digits[i];
        os.fill(old_fill);
    }

    return os;
}

bool operator ==(const BigInt& left, const BigInt& right) {
    if (left.neg != right.neg) return false;
    if (left.digits.empty()) {
        if (right.digits.empty() || (right.digits.size() == 1 && right.digits[0] == 0)) return true;
        else return false;
    }

    if (right.digits.empty()) {
        if (left.digits.size() == 1 && left.digits[0] == 0) return true;
        else return false;
    }

    if (left.digits.size() != right.digits.size()) return false;
    for (size_t i = 0; i < left.digits.size(); ++i) if (left.digits[i] != right.digits[i]) return false;

    return true;
}

bool operator <(const BigInt& left, const BigInt& right) {
    if (left == right) return false;
    if (left.neg) {
        if (right.neg) return ((-right) < (-left));
        else return true;
    }
    else if (right.neg) return false;
    else {
        if (left.digits.size() != right.digits.size()) {
            return left.digits.size() < right.digits.size();
        }
        else {
            for (long long i = left.digits.size() - 1; i >= 0; --i) {
                if (left.digits[i] != right.digits[i]) return left.digits[i] < right.digits[i];
            }

            return false;
        }
    }
}

bool operator >(const BigInt& left, const BigInt& right) {
    return !(left < right || left == right);
}

BigInt BigInt::operator +() const {
    return BigInt(*this);
}

BigInt BigInt::operator -() const {
    BigInt copy(*this);
    copy.neg = !copy.neg;
    return copy;
}

BigInt operator +(BigInt left, const BigInt& right) {
    if (left.neg) {
        if (right.neg) return -(-left + (-right));
        else return right - (-left);
    }
    else if (right.neg) return left - (-right);
    int carry = 0;
    for (size_t i = 0; i < std::max(left.digits.size(), right.digits.size()) || carry != 0; ++i) {
        if (i == left.digits.size()) left.digits.push_back(0);
        left.digits[i] += carry + (i < right.digits.size() ? right.digits[i] : 0);
        carry = left.digits[i] >= BigInt::base;
        if (carry != 0) left.digits[i] -= BigInt::base;
    }

    return left;
}

BigInt operator -(BigInt left, const BigInt& right) {
    if (right.neg) return left + (-right);
    else if (left.neg) return -(-left + right);
    else if (left < right) return -(right - left);
    int carry = 0;
    for (size_t i = 0; i < right.digits.size() || carry != 0; ++i) {
        left.digits[i] -= carry + (i < right.digits.size() ? right.digits[i] : 0);
        carry = left.digits[i] < 0;
        if (carry != 0) left.digits[i] += BigInt::base;
    }

    left.removeZeros();
    return left;
}

BigInt operator *(const BigInt& left, const BigInt& right) {
    BigInt result;
    result.digits.resize(left.digits.size() + right.digits.size());
    for (size_t i = 0; i < left.digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < right.digits.size() || carry != 0; ++j) {
            long long cur = result.digits[i + j] +
                            left.digits[i] * 1LL * (j < right.digits.size() ? right.digits[j] : 0) + carry;
            result.digits[i + j] = static_cast<int>(cur % BigInt::base);
            carry = static_cast<int>(cur / BigInt::base);
        }
    }

    result.neg = left.neg != right.neg;
    result.removeZeros();
    return result;
}

void BigInt::_shift_right() {
    if (this->digits.empty()) {
        this->digits.push_back(0);
        return;
    }
    this->digits.push_back(this->digits[this->digits.size() - 1]);
    for (size_t i = this->digits.size() - 2; i > 0; --i) this->digits[i] = this->digits[i - 1];
    this->digits[0] = 0;
}

BigInt operator /(const BigInt& left, const BigInt& right) {
    if (right.empty()) throw BigInt::divide_by_zero();
    BigInt b = right;
    b.neg = false;
    BigInt result, current;
    result.digits.resize(left.digits.size());
    for (long long i = static_cast<long long>(left.digits.size()) - 1; i >= 0; --i) {
        current._shift_right();
        current.digits[0] = left.digits[i];
        current.removeZeros();
        int x = 0, l = 0, r = BigInt::base;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInt t = b * m;
            if (t < current || t == current) {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }

        result.digits[i] = x;
        current = current - b * x;
    }

    result.neg = left.neg != right.neg;
    result.removeZeros();
    return result;
}

bool BigInt::empty() const{
    return this->digits.empty() || (this->digits[0] == 0 && this->digits.size() == 1);
}

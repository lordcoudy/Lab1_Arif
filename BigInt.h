//
// Created by Milord on 9/14/23.
//
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#ifndef LAB1_ARIF_BIGINT_H
#define LAB1_ARIF_BIGINT_H

typedef long long ll;

class BigInt
{
    static const int base = 1000000000;
    std::vector<int> digits;
    bool neg;

    void removeZeros();
    void _shift_right();
    bool empty() const;

public:
    class divide_by_zero: public std::exception { };
    class overflow: public std::exception { };

    BigInt();
    BigInt(std::string);
    BigInt(signed char);
    BigInt(unsigned char);
    BigInt(signed short);
    BigInt(unsigned short);
    BigInt(signed int);
    BigInt(unsigned int);
    BigInt(signed long);
    BigInt(unsigned long);
    BigInt(signed long long);
    BigInt(unsigned long long);

    friend std::ostream& operator <<(std::ostream&, const BigInt&);
    friend bool operator ==(const BigInt&, const BigInt&);
    friend bool operator <(const BigInt&, const BigInt&);
    friend bool operator >(const BigInt&, const BigInt&);
    operator std::string() const;
    BigInt operator +() const;
    BigInt operator -() const;
    friend BigInt operator +(BigInt, const BigInt&);
    friend BigInt operator -(BigInt, const BigInt&);
    friend BigInt operator *(const BigInt&, const BigInt&);
    friend BigInt operator /(const BigInt&, const BigInt&);

};


#endif //LAB1_ARIF_BIGINT_H

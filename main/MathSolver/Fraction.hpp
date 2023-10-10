#pragma once
#include <stdexcept>
#include <string>

#include "BigInt.hpp"


class BigInt;
class Fraction {
public:
    class NullDenominatorException : public std::runtime_error {
    public:
        NullDenominatorException() : std::runtime_error("attempting to create null-denominator fraction") {}
    };
private:
    BigInt m_numerator;
    BigInt m_denominator;
public:
    Fraction(BigInt numerator, BigInt denominator);
    ~Fraction();
public:
    Fraction operator+(const Fraction& other) const;
    Fraction operator-(const Fraction& other) const;
    Fraction operator*(const Fraction& other) const;
    Fraction operator/(const Fraction& other) const;
    bool operator==(const Fraction& other) const;
public:
    BigInt getNumberator() const;
    BigInt getDenominator() const;
    bool isNegative() const;
    bool isZero() const;
    Fraction factorial() const;
    std::string toString(bool ignoreNegative) const;
    Fraction gcd_fraction(const Fraction& other) const;
};
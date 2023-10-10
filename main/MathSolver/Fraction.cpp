#include "Fraction.hpp"

#include <cassert>
#include <numeric>

Fraction::Fraction(BigInt numerator, BigInt denominator) {
    if (denominator == 0) {
        throw Fraction::NullDenominatorException();
    }
    int multiplier = denominator < 0 ? -1 : 1;
    BigInt gcd_num = gcd(numerator, denominator);
    this->m_numerator = numerator / gcd_num * multiplier;
    this->m_denominator = denominator / gcd_num * multiplier;
}

Fraction::~Fraction() {}
Fraction Fraction::operator+(const Fraction& other) const {
    BigInt lcm_num = lcm(this->m_denominator, other.m_denominator);
    return Fraction((this->m_numerator * (lcm_num / this->m_denominator)) + (other.m_numerator * (lcm_num / other.m_denominator)), lcm_num);
}
Fraction Fraction::operator-(const Fraction& other) const {
    BigInt lcm_num = lcm(this->m_denominator, other.m_denominator);
    return Fraction((this->m_numerator * (lcm_num / this->m_denominator)) - (other.m_numerator * (lcm_num / other.m_denominator)), lcm_num);
}
Fraction Fraction::operator*(const Fraction& other) const {
    return Fraction(this->m_numerator * other.m_numerator, this->m_denominator * other.m_denominator);
}
Fraction Fraction::operator/(const Fraction& other) const {
    return Fraction(this->m_numerator * other.m_denominator, this->m_denominator * other.m_numerator);
}
bool Fraction::operator==(const Fraction& other) const {
    return this->m_numerator == other.m_numerator && this->m_denominator == other.m_denominator;
}
bool Fraction::isNegative() const {
    return this->m_numerator < 0;
}
bool Fraction::isZero() const {
    return this->m_numerator == 0;
}
Fraction Fraction::factorial() const {
    if(this->m_denominator != 1 || this->m_numerator <= 0){
        throw std::runtime_error("can only factorial natural numbers");
    }
    BigInt running_count = 1;
    for(BigInt i = 1;i <= this->m_numerator;i++){
        running_count *= i;
    }
    return Fraction(running_count, 1);
}
BigInt Fraction::getNumberator() const {
    return this->m_numerator;
}
BigInt Fraction::getDenominator() const {
    return this->m_denominator;
}
std::string Fraction::toString(bool ignoreNegative) const {
    std::string output;
    output += (ignoreNegative ? abs(m_numerator) : m_numerator).to_string();
    if (m_denominator != 1) {
        output += "/" + m_denominator.to_string();
    }
    return output;
}

Fraction Fraction::gcd_fraction(const Fraction& other) const {
    return Fraction(gcd(m_numerator, other.m_numerator), lcm(m_denominator, other.m_denominator));
}
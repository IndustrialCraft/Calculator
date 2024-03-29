#include "Expression.hpp"

#include <iostream>

#include "Term.hpp"

Expression::~Expression() {}

Expression Expression::operator+(const Term& term) const {
    std::vector<Term> terms = m_terms;
    terms.push_back(term);
    return Expression(terms);
}
Expression Expression::operator*(const Term& term) const {
    std::vector<Term> terms;
    for (const auto& term_it : m_terms) {
        terms.push_back(term_it * term);
    }
    return terms;
}
Expression Expression::operator/(const Term& term) const {
    std::vector<Term> terms;
    for (const auto& term_it : m_terms) {
        terms.push_back(term_it / term);
    }
    return terms;
}
Expression Expression::operator+(const Expression& other) const {
    std::vector<Term> terms = m_terms;
    for (const Term& term : other.m_terms) terms.push_back(term);
    return Expression(terms);
}
Expression Expression::operator-(const Expression& other) const {
    std::vector<Term> terms = m_terms;
    for (const Term& term : other.m_terms) terms.push_back(term * Fraction(-1, 1));
    return Expression(terms);
}
Expression Expression::operator*(const Expression& other) const {
    std::vector<Term> terms;
    for (const Term& term1 : this->m_terms) {
        for (const Term& term2 : other.m_terms) {
            terms.push_back(term1 * term2);
        }
    }
    return Expression(terms);
}
Expression Expression::simplify() const {
    std::vector<Term> terms;
    Expression simplifiedTerms = simplifyTerms();
    for (const Term& term : simplifiedTerms.m_terms) {
        if (term.getAmount().isZero())
            continue;
        bool found = false;
        for (int i = 0; i < terms.size(); i++) {
            if (term.isSameExceptAmount(terms.at(i))) {
                terms.at(i) = term + terms.at(i).getAmount();
                if (terms.at(i).getAmount().isZero())
                    terms.erase(terms.begin() + i);
                found = true;
                break;
            }
        }
        if (!found) {
            terms.push_back(term);
        }
    }
    return Expression(terms);
}
std::set<Variable> Expression::listVariables() const {
    std::set<Variable> variables;
    for (const Term& term : m_terms) {
        auto extracted = term.listVariables(true);
        variables.insert(extracted.begin(), extracted.end());
    }
    return variables;
}
Expression Expression::replaceVariable(Variable variable, Expression expression) const {
    std::vector<Term> terms;
    for (const Term& term : m_terms) {
        terms.push_back(term.replaceVariable(variable, expression));
    }
    return Expression(terms);
}
Expression Expression::simplifyTerms() const {
    std::vector<Term> terms;
    for (const Term& term : m_terms) {
        for (const Term& term2 : term.simplify()) {
            terms.push_back(term2);
        }
    }
    return terms;
}
std::vector<Term> Expression::getTerms() const {
    return this->m_terms;
}
std::string Expression::toString() const {
    std::string output;
    bool isFirst = true;
    for (const Term& term1 : this->m_terms) {
        if ((!isFirst) || term1.getAmount().isNegative())
            output += term1.getAmount().isNegative() ? "-" : "+";
        output += term1.toString(true);
        isFirst = false;
    }
    return output;
}
Term Expression::getCommonVarsAndCount() const {
    Fraction gcd(0, 0);
    Expression simplified = simplify();
    for (const Term& term : simplified.m_terms) {
        if (gcd.isZero())
            gcd = term.getAmount();
        else
            gcd = gcd.gcd_fraction(term.getAmount());
    }
    // todo
    return Term(Fraction(1, 1));
}
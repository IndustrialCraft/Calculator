#include "ExpressionReader.hpp"

#include <cctype>

#include "ESPIO.hpp"
#include "Term.hpp"

Expression ExpressionReader::parse(std::string input) {
    return parseExpr(input + ")", 0).first;
}

std::pair<Expression, int> ExpressionReader::parseExpr(std::string input, int offset) {
    Expression expr;
    bool first = true;
    bool isSub = false;
    bool isInverse = false;
    Term currentTerm(Fraction(1, 1));
    for (int i = offset; i < input.length(); i++) {
        char ch = input.at(i);
        if (ch == '*')
            continue;
        if (ch == '/') {
            isInverse = true;
            continue;
        }
        if (isdigit(ch)) {
            std::string num = "";
            for (; isdigit(input.at(i)); i++) {
                num = num + input.at(i);
            }
            i--;
            Fraction number = Fraction(std::stoi(num),1);
            if(input.at(i+1) == '!'){
                number = number.factorial();
                i++;
            }
            auto times = ExpressionReader::tryParseUpperNum(input, i + 1);
            for (int i = 0; i < times.first; i++) currentTerm = isInverse ? (currentTerm / number) : (currentTerm * number);
            i = times.second - 1;
            isInverse = false;
            first = false;
        }
        if (isalpha(ch)) {
            auto times = ExpressionReader::tryParseUpperNum(input, i + 1);
            for (int i = 0; i < times.first; i++) currentTerm = isInverse ? (currentTerm / Variable(ch)) : (currentTerm * Variable(ch));
            i = times.second - 1;
            isInverse = false;
            first = false;
            continue;
        }
        if (ch == '(') {
            std::pair<Expression, int> ePair = parseExpr(input, i + 1);
            auto times = ExpressionReader::tryParseUpperNum(input, ePair.second + 1);
            for (int i = 0; i < times.first; i++) currentTerm = isInverse ? (currentTerm / ePair.first) : (currentTerm * ePair.first);
            i = times.second - 1;
            isInverse = false;
            first = false;
            continue;
        }
        if (ch == '+') {
            if (!first)
                expr = (expr + (currentTerm * (isSub ? Fraction(-1, 1) : Fraction(1, 1))));
            isSub = false;
            first = false;
            isInverse = false;
            currentTerm = Term(Fraction(1, 1));
            continue;
        }
        if (ch == '-') {
            if (!first)
                expr = (expr + (currentTerm * (isSub ? Fraction(-1, 1) : Fraction(1, 1))));
            isSub = true;
            first = false;
            isInverse = false;
            currentTerm = Term(Fraction(1, 1));
            continue;
        }
        if (ch == ')') {
            if (!first)
                expr = (expr + (currentTerm * (isSub ? Fraction(-1, 1) : Fraction(1, 1))));
            return std::pair<Expression, int>(expr, i);
        }
    }
    throw std::logic_error("exprected )");
}

std::pair<int, int> ExpressionReader::tryParseUpperNum(std::string input, int i) {
    std::string num;
    for (; ESPIO::isUpperNum(input.at(i)); i++) {
        num = num + input.at(i);
    }
    if (num.empty()) {
        return std::pair<int, int>(1, i);
    }
    return std::pair<int, int>(ESPIO::upperNumToInt(num), i);
}
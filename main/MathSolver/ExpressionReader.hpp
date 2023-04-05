#pragma once
#include <optional>
#include <string>

#include "Expression.hpp"

class ExpressionReader {
public:
    Expression parse(std::string input);
private:
    std::pair<Expression, int> parseExpr(std::string input, int offset);
    std::pair<int, int> tryParseUpperNum(std::string input, int i);
};
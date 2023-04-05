#include "BasicCalculatorMenu.hpp"

#include <iostream>

#include "ExpressionPair.hpp"
#include "HistoryManager.hpp"
#include "HistoryMenu.hpp"
#include "MainMenu.hpp"
#include "MathSolver/ExpressionReader.hpp"
#include "MathSolver/Term.hpp"

std::vector<std::string> splitString(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}
ExpressionPair::EOperator charToOperator(char ch) {
    if (ch == '=')
        return ExpressionPair::EOperator::EQUALS;
    if (ch == '>')
        return ExpressionPair::EOperator::GREATER;
    if (ch == '<')
        return ExpressionPair::EOperator::LESS;
    throw std::logic_error("invalid operator code");
}
char operatorToChar(ExpressionPair::EOperator op) {
    if (op == ExpressionPair::EOperator::EQUALS)
        return '=';
    if (op == ExpressionPair::EOperator::GREATER)
        return '>';
    if (op == ExpressionPair::EOperator::LESS)
        return '<';
    throw std::logic_error("invalid operator code");
}

BasicCalculatorMenu::BasicCalculatorMenu() {}
BasicCalculatorMenu::BasicCalculatorMenu(std::string input) : m_textfield(input) {}

BasicCalculatorMenu::~BasicCalculatorMenu() {}

void BasicCalculatorMenu::tick(MenuSelector& menuSelector, IO& io) {
    io.setCursorVisibility(true);
    IO::EInput input = io.getInput();
    if (input == IO::EInput::back)
        menuSelector.setMenu(new MainMenu());
    if (input != IO::EInput::none) {
        m_textfield.input(input);
    }
    if (input == IO::EInput::history)
        menuSelector.setMenu(new HistoryMenu());
    if (input == IO::EInput::exec) {
        HistoryManager::getInstance().addHistory(m_textfield.getText());
        char found = 0;
        if (m_textfield.getText().find('=') != std::string::npos)
            found = '=';
        if (m_textfield.getText().find('>') != std::string::npos)
            found = '>';
        if (m_textfield.getText().find('<') != std::string::npos)
            found = '<';
        if (found == 0) {
            m_textfield.setText(ExpressionReader().parse(m_textfield.getText()).simplify().toString(), true);
        } else {
            auto split = splitString(m_textfield.getText(), std::string(1, found));
            if (split.size() != 2)
                return;
            Expression expr1 = ExpressionReader().parse(split.at(0));
            Expression expr2 = ExpressionReader().parse(split.at(1));
            auto vars = expr1.listVariables();
            auto vars2 = expr2.listVariables();
            vars.insert(vars2.begin(), vars2.end());
            ExpressionPair ep(expr1, expr2, charToOperator(found));
            ep = ep.solveFor(*vars.begin());
            m_textfield.setText(ep.getFirst().toString() + operatorToChar(ep.getOperator()) + ep.getSecond().toString(), true);
        }
    }
    io.writeString("Basic Calculator", 0, 0);
    m_textfield.draw(io, 1);
}
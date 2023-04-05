#include "QuadraticCalculator.hpp"

#include <cmath>
#include <sstream>
#include <vector>

#include "MainMenu.hpp"
#include "MathSolver/Fraction.hpp"

QuadraticCalculator::QuadraticCalculator() {}

QuadraticCalculator::~QuadraticCalculator() {}

std::vector<std::string> splitString(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

Fraction readFraction(std::string str) {
    std::string firstDigit;
    std::string secondDigit;
    bool isSecond = false;
    for (int i = 0; i < str.size(); i++) {
        char ch = str.at(i);
        if (ch == '/') {
            isSecond = true;
            continue;
        }
        if (isSecond) {
            secondDigit += ch;
        } else {
            firstDigit += ch;
        }
    }
    if (secondDigit.empty())
        secondDigit = "1";
    return Fraction(std::stoi(firstDigit), std::stoi(secondDigit));
}

void QuadraticCalculator::tick(MenuSelector& menuSelector, IO& io) {
    io.setCursorVisibility(true);
    IO::EInput input = io.getInput();
    if (input == IO::EInput::back)
        menuSelector.setMenu(new MainMenu());
    if (input == IO::EInput::exec) {
        std::vector<std::string> split = splitString(m_textfield.getText(), ',');
        if (split.size() != 3)
            return;
        Fraction a = readFraction(split[0]);
        Fraction b = readFraction(split[1]);
        Fraction c = readFraction(split[2]);

        Fraction d = (b * b) - (a * c * Fraction(4, 1));
        if (d.isZero()) {
            m_textfield.setText("K={" + ((Fraction(-1, 1) * b) / (Fraction(2, 1) * a)).toString(false) + "}", true);
        } else if (d.isNegative()) {
            m_textfield.setText(std::string("K=") + ((char)0xAE), true);
        } else {
            if (d.getDenominator() == 1) {
                float dSqrt = sqrt(d.getNumberator());
                if (dSqrt == ((int)dSqrt)) {
                    m_textfield.setText("K={" + (((Fraction(-1, 1) * b) - Fraction(dSqrt, 1)) / (Fraction(2, 1) * a)).toString(false) + "," + (((Fraction(-1, 1) * b) + Fraction(dSqrt, 1)) / (Fraction(2, 1) * a)).toString(false) + "}", true);
                } else {
                }
            }
        }
    } else {
        m_textfield.input(input);
    }
    io.writeString("Quadratic Calc", 0, 0);
    m_textfield.draw(io, 1);
}
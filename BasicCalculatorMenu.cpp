#include "BasicCalculatorMenu.hpp"

BasicCalculatorMenu::BasicCalculatorMenu() {}

BasicCalculatorMenu::~BasicCalculatorMenu() {}

void BasicCalculatorMenu::tick(MenuSelector& menuSelector, IO& io) {
    IO::EInput input = io.getInput();
    if (input != IO::EInput::none) {
        m_textfield.input(input);
    }
    io.writeString("Basic Calculator", 0, 0);
    m_textfield.draw(io, 1);
}
#include "BasicCalculatorMenu.hpp"

BasicCalculatorMenu::BasicCalculatorMenu() {}

BasicCalculatorMenu::~BasicCalculatorMenu() {}

void BasicCalculatorMenu::tick(MenuSelector& menuSelector, IO& io) {
    io.writeString("Basic Calculator", 0, 0);
}
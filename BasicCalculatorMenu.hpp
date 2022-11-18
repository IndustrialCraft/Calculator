#pragma once
#include "Menu.hpp"

class BasicCalculatorMenu : public Menu {
public:
    BasicCalculatorMenu();
    ~BasicCalculatorMenu();
public:
    virtual void tick(MenuSelector& menuSelector, IO& io) override;
};
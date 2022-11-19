#pragma once
#include "Menu.hpp"
#include "TextField.hpp"

class BasicCalculatorMenu : public Menu {
private:
    TextField m_textfield;
public:
    BasicCalculatorMenu();
    ~BasicCalculatorMenu();
public:
    virtual void tick(MenuSelector& menuSelector, IO& io) override;
};
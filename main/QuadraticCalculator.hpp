#pragma once

#include "Menu.hpp"
#include "TextField.hpp"

class QuadraticCalculator : public Menu {
private:
    TextField m_textfield;
public:
    QuadraticCalculator();
    ~QuadraticCalculator();
public:
    virtual void tick(MenuSelector& menuSelector, IO& io) override;
};
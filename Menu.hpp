#pragma once
#include "IO.hpp"
#include "MenuSelector.hpp"

class Menu {
public:
    Menu();
    ~Menu();
public:
    virtual void tick(MenuSelector& menuSelector, IO& io) = 0;
};
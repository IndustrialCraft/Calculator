#pragma once
#include "IO.hpp"
#include "MenuSelector.hpp"

class Menu {
public:
    Menu();
    virtual ~Menu();
public:
    virtual void tick(MenuSelector& menuSelector, IO& io) = 0;
};
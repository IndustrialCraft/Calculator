#pragma once

#include "Menu.hpp"

class HistoryMenu : public Menu {
private:
    int m_current;
public:
    HistoryMenu();
    ~HistoryMenu();
public:
    virtual void tick(MenuSelector& menuSelector, IO& io) override;
};
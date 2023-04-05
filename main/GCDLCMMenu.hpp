#pragma once
#include "Menu.hpp"
#include "TextField.hpp"

class GCDLCMMenu : public Menu {
private:
    TextField m_textfield;
    bool m_isLCM;
public:
    GCDLCMMenu(bool isLCM);
    ~GCDLCMMenu();
public:
    virtual void tick(MenuSelector& menuSelector, IO& io) override;
};
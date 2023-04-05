#pragma once

#include "Menu.hpp"
#include "TextField.hpp"

class ExceptionMenu : public Menu {
private:
    TextField m_text;
public:
    ExceptionMenu(std::string text);
    ~ExceptionMenu();
public:
    virtual void tick(MenuSelector& menuSelector, IO& io) override;
};
#pragma once

#include <memory>

class Menu;

class MenuSelector {
private:
    std::unique_ptr<Menu> m_menu;
public:
    MenuSelector();
    ~MenuSelector();
public:
    Menu& getMenu();
    void setMenu(Menu* menu);
};
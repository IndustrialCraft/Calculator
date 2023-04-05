#pragma once

#include <memory>

class Menu;

class MenuSelector {
private:
    Menu* m_scheduledMenu;
    std::unique_ptr<Menu> m_menu;
public:
    MenuSelector();
    ~MenuSelector();
public:
    void tick();
    Menu& getMenu();
    void setMenu(Menu* menu);
};
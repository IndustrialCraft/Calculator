#include "MenuSelector.hpp"

#include "Menu.hpp"

MenuSelector::MenuSelector() : m_menu(nullptr) {}

MenuSelector::~MenuSelector() {}

Menu& MenuSelector::getMenu() {
    return *m_menu;
}
void MenuSelector::setMenu(Menu* menu) {
    this->m_menu = std::unique_ptr<Menu>(menu);
}
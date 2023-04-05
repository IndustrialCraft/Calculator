#include "MenuSelector.hpp"

#include "Menu.hpp"

MenuSelector::MenuSelector() : m_scheduledMenu(nullptr), m_menu(nullptr) {}

MenuSelector::~MenuSelector() {}

void MenuSelector::tick() {
    if (m_scheduledMenu != nullptr) {
        m_menu = std::unique_ptr<Menu>(m_scheduledMenu);
        m_scheduledMenu = nullptr;
    }
}

Menu& MenuSelector::getMenu() {
    return *m_menu;
}
void MenuSelector::setMenu(Menu* menu) {
    if (m_scheduledMenu != nullptr)
        delete m_scheduledMenu;
    m_scheduledMenu = menu;
}
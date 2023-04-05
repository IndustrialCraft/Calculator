#include "HistoryMenu.hpp"

#include "BasicCalculatorMenu.hpp"
#include "HistoryManager.hpp"
#include "MainMenu.hpp"

HistoryMenu::HistoryMenu() : m_current(0) {}

HistoryMenu::~HistoryMenu() {}

void HistoryMenu::tick(MenuSelector& menuSelector, IO& io) {
    const auto& history = HistoryManager::getInstance().getHistory();
    io.setCursorVisibility(false);
    IO::EInput input = io.getInput();
    if (input == IO::EInput::back)
        menuSelector.setMenu(new MainMenu());
    if (input == IO::EInput::up)
        m_current++;
    if (input == IO::EInput::down)
        m_current--;
    if (input == IO::EInput::exec) {
        if (history.size() > 0)
            menuSelector.setMenu(new BasicCalculatorMenu(history.at(m_current)));
        else
            menuSelector.setMenu(new BasicCalculatorMenu());
    }
    if (m_current < 0)
        m_current = history.size() - 1;
    if (m_current >= history.size())
        m_current = 0;
    io.writeString("History", 0, 0);
    for (int i = 0; i < 3; i++) {
        if (history.size() > m_current + i)
            io.writeString(history.at(m_current + i), 0, i + 1);
    }
}
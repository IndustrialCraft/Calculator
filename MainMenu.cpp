#include "MainMenu.hpp"

#include "BasicCalculatorMenu.hpp"

typedef std::pair<std::string, std::function<void(MenuSelector&)>> Page;

MainMenu::MainMenu() : m_currentPage(0) {
    this->m_pages.push_back(Page("Basic Calculator", [](MenuSelector& menuSelector) { menuSelector.setMenu(new BasicCalculatorMenu()); }));
    this->m_pages.push_back(Page("Equation System", [](MenuSelector& menuSelector) {}));
    this->m_pages.push_back(Page("Root", [](MenuSelector& menuSelector) {}));
}

MainMenu::~MainMenu() {}

void MainMenu::tick(MenuSelector& menuSelector, IO& io) {
    Page& page = m_pages.at(m_currentPage);
    io.writeString(page.first, 10 - (page.first.size() / 2), 1);
    IO::EInput input = io.getInput();
    if (input == IO::EInput::left) {
        m_currentPage--;
    } else if (input == IO::EInput::right) {
        m_currentPage++;
    } else if (input == IO::EInput::exec) {
        page.second(menuSelector);
    }
    m_currentPage = (m_currentPage + m_pages.size()) % m_pages.size();
}
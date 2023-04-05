#include "CharacterListingMenu.hpp"

#include <sstream>

#include "MainMenu.hpp"

CharacterListingMenu::CharacterListingMenu() : m_page(0) {}

CharacterListingMenu::~CharacterListingMenu() {}

void CharacterListingMenu::tick(MenuSelector& menuSelector, IO& io) {
    IO::EInput input = io.getInput();
    if (input == IO::EInput::back)
        menuSelector.setMenu(new MainMenu());
    if (input == IO::EInput::left)
        m_page--;
    if (input == IO::EInput::right)
        m_page++;
    if (m_page < 0)
        m_page = 15;
    if (m_page > 15)
        m_page = 0;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            int charIndex = (m_page * 16) + y + (x * 4);
            std::ostringstream hexstream;
            hexstream << std::hex << charIndex;
            std::string result = hexstream.str();
            io.writeString(result + ":" + ((char)charIndex) + " ", x * 5, y);
        }
    }
}
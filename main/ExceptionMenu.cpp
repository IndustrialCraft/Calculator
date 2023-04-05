#include "ExceptionMenu.hpp"

#include "MainMenu.hpp"

ExceptionMenu::ExceptionMenu(std::string text) : m_text(text) {
    m_text.setCanEdit(false);
}
ExceptionMenu::~ExceptionMenu() {}

void ExceptionMenu::tick(MenuSelector& menuSelector, IO& io) {
    io.writeString("Exception Catched", 0, 0);
    io.setCursorVisibility(true);
    IO::EInput input = io.getInput();
    if (input == IO::EInput::back)
        menuSelector.setMenu(new MainMenu());
    m_text.input(input);
    m_text.draw(io, 2);
}
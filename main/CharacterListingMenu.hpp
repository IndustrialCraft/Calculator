#pragma once
#include "Menu.hpp"

class CharacterListingMenu : public Menu {
private:
    int m_page;
public:
    CharacterListingMenu();
    ~CharacterListingMenu();
public:
    virtual void tick(MenuSelector& menuSelector, IO& io) override;
};
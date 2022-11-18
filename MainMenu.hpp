#pragma once
#include <functional>
#include <vector>

#include "Menu.hpp"

class MainMenu : public Menu {
private:
    std::vector<std::pair<std::string, std::function<void(MenuSelector&)>>> m_pages;
    int m_currentPage;
public:
    MainMenu();
    ~MainMenu();
public:
    virtual void tick(MenuSelector& menuSelector, IO& io) override;
};
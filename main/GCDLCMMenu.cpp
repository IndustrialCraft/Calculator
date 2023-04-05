#include "GCDLCMMenu.hpp"

#include <algorithm>
#include <numeric>

#include "MainMenu.hpp"

GCDLCMMenu::GCDLCMMenu(bool isLCM) : m_isLCM(isLCM) {}

GCDLCMMenu::~GCDLCMMenu() {}

int lcm(std::vector<int> arr) {
    int ans = arr[0];

    for (int i = 1; i < arr.size(); i++) ans = (((arr[i] * ans)) / (std::gcd(arr[i], ans)));

    return ans;
}

void GCDLCMMenu::tick(MenuSelector& menuSelector, IO& io) {
    io.setCursorVisibility(true);
    IO::EInput input = io.getInput();
    if (input == IO::EInput::back)
        menuSelector.setMenu(new MainMenu());
    if (input != IO::EInput::none) {
        m_textfield.input(input);
    }
    if (input == IO::EInput::exec) {
        std::string text = m_textfield.getText() + ",";
        std::vector<int> numbers;
        std::string toParse = "";
        for (const char& ch : text) {
            if (ch == ',') {
                numbers.push_back(std::stoi(toParse));
                toParse = "";
            } else {
                toParse += ch;
            }
        }
        if (m_isLCM) {
            m_textfield.setText(std::to_string(lcm(numbers)), true);
        } else {
            auto result = numbers[0];
            for (int i = 1; i < numbers.size(); ++i) result = std::gcd(result, numbers[i]);
            m_textfield.setText(std::to_string(result), true);
        }
    }
    io.writeString(std::string(m_isLCM ? "LCM" : "GCD") + " Calculator", 0, 0);
    m_textfield.draw(io, 1);
}
#pragma once

#include <utility>

#include "IO.hpp"

class KeyboardIO : public IO {
private:
    char m_display[20][4];
    bool m_alpha;
    std::pair<int, int> m_cursor;
    bool m_isCursorVisible;
    EInput m_input;
public:
    KeyboardIO();
    ~KeyboardIO();
public:
    virtual void setCursor(int x, int y) override;
    virtual void setCursorVisibility(bool visible) override;
    virtual void tick() override;
    virtual void setChar(int x, int y, char ch) override;
    virtual void flushDisplay() override;
    virtual EInput getInput() override;
private:
    void clearScreen();
};
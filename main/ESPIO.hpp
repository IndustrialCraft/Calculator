#pragma once

#include <utility>

#include "IO.hpp"

class ESPIO : public IO {
public:
    enum EInputMode {
        none,
        shift,
        alpha,
        power,
    };
private:
    char m_display[20][4];
    char m_lastDisplay[20][4];
    EInputMode m_inputMode;
    int m_activateJoystickTimer;
    int m_joystickLastRepeatActivation;
    uint32_t m_lastInput;
    std::pair<int, int> m_cursor;
    bool m_isCursorVisible;
    EInput m_input;
public:
    ESPIO();
    ~ESPIO();
public:
    virtual void setCursor(int x, int y) override;
    virtual void setCursorVisibility(bool visible) override;
    virtual void tick() override;
    virtual void setChar(int x, int y, char ch) override;
    virtual void flushDisplay() override;
    virtual EInput getInput() override;
public:
    static std::string intToUpperNum(int input);
    static int upperNumToInt(std::string input);
    static bool isUpperNum(char ch);
private:
    void clearScreen();
    uint8_t compareAndCopyDisplayBuffers();
};
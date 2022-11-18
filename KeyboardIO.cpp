#include "KeyboardIO.hpp"

#include <iostream>
#include <stdexcept>

#include "ncurses.h"

KeyboardIO::KeyboardIO() : m_alpha(false), m_cursor(0, 0), m_isCursorVisible(false), m_input(EInput::none) {
    clearScreen();
    WINDOW* handle = initscr();
    nocbreak();
    nodelay(handle, true);
    cbreak();
    noecho();
    refresh();
}

KeyboardIO::~KeyboardIO() {
    endwin();
}
void KeyboardIO::setCursor(int x, int y) {
    this->m_cursor = std::pair<int, int>(x, y);
}
void KeyboardIO::setCursorVisibility(bool visible) {
    this->m_isCursorVisible = visible;
}
void KeyboardIO::tick() {
    char ch = tolower(getch());
    if (ch == ERR)
        return;
    if (m_alpha) {
        switch (ch) {
            case 'a':
                this->m_input = EInput::ka;
                break;
            case 'b':
                this->m_input = EInput::kb;
                break;
            case 'c':
                this->m_input = EInput::kc;
                break;
            case 'd':
                this->m_input = EInput::kd;
                break;
            case 'e':
                this->m_input = EInput::ke;
                break;
            case 'f':
                this->m_input = EInput::kf;
                break;
            case 'g':
                this->m_input = EInput::kg;
                break;
            case 'h':
                this->m_input = EInput::kh;
                break;
            case 'i':
                this->m_input = EInput::ki;
                break;
            case 'j':
                this->m_input = EInput::kj;
                break;
            case 'k':
                this->m_input = EInput::kk;
                break;
            case 'l':
                this->m_input = EInput::kl;
                break;
            case 'm':
                this->m_input = EInput::km;
                break;
            case 'n':
                this->m_input = EInput::kn;
                break;
            case 'o':
                this->m_input = EInput::ko;
                break;
            case 'p':
                this->m_input = EInput::kp;
                break;
            case 'r':
                this->m_input = EInput::kr;
                break;
            case 's':
                this->m_input = EInput::ks;
                break;
            case 't':
                this->m_input = EInput::kt;
                break;
            case 'u':
                this->m_input = EInput::ku;
                break;
            case 'v':
                this->m_input = EInput::kv;
                break;
            case 'x':
                this->m_input = EInput::kx;
                break;
            case 'y':
                this->m_input = EInput::ky;
                break;
            case 'z':
                this->m_input = EInput::kz;
                break;
        }
        m_alpha = false;
    } else {
        switch (ch) {
            case 'a':
                m_alpha = true;
                return;
            case 'o':
                this->m_input = EInput::paranOpen;
                return;
            case 'p':
                this->m_input = EInput::paranClose;
                return;
            case 'z':
                this->m_input = EInput::less;
                return;
            case 'u':
                this->m_input = EInput::equals;
                return;
            case 'i':
                this->m_input = EInput::more;
                return;
            case '+':
                this->m_input = EInput::plus;
                return;
            case '-':
                this->m_input = EInput::minus;
                return;
            case '*':
                this->m_input = EInput::times;
                return;
            case '/':
                this->m_input = EInput::divide;
                return;
            case 'h':
                this->m_input = EInput::history;
                return;
            case 'm':
                this->m_input = EInput::menu;
                return;
            case 'e':
                this->m_input = EInput::exec;
                return;
            case 'b':
                this->m_input = EInput::back;
                return;
            case 'r':
                this->m_input = EInput::left;
                return;
            case 't':
                this->m_input = EInput::right;
                return;
            case '0':
                this->m_input = EInput::n0;
                return;
            case '1':
                this->m_input = EInput::n1;
                return;
            case '2':
                this->m_input = EInput::n2;
                return;
            case '3':
                this->m_input = EInput::n3;
                return;
            case '4':
                this->m_input = EInput::n4;
                return;
            case '5':
                this->m_input = EInput::n5;
                return;
            case '6':
                this->m_input = EInput::n6;
                return;
            case '7':
                this->m_input = EInput::n7;
                return;
            case '8':
                this->m_input = EInput::n8;
                return;
            case '9':
                this->m_input = EInput::n9;
                return;
        }
    }
}
void KeyboardIO::setChar(int x, int y, char ch) {
    if (x < 0 || x >= 20 || y < 0 || y >= 4)
        throw std::range_error("character assign outside boundaries");
    m_display[x][y] = ch;
}
void KeyboardIO::flushDisplay() {
    curs_set(m_isCursorVisible);
    move(m_cursor.second, m_cursor.first);
    for (int y = 0; y < 4; y++) {
        move(y, 0);
        for (int x = 0; x < 20; x++) {
            addch(m_display[x][y]);
        }
    }
    clearScreen();
    refresh();
}
IO::EInput KeyboardIO::getInput() {
    EInput input = this->m_input;
    this->m_input = EInput::none;
    return input;
}

void KeyboardIO::clearScreen() {
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 4; y++) {
            m_display[x][y] = ' ';
        }
    }
}
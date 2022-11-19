#include "TextField.hpp"

#include <algorithm>

TextField::TextField(std::string text) : m_text(text), m_cursor(0) {}

TextField::~TextField() {}

int TextField::getCursor() {
    return m_cursor;
}
std::string TextField::getText() {
    return m_text;
}
void TextField::setText(std::string text, bool resetCursor) {
    this->m_text = text;
    if (resetCursor) {
        this->m_cursor = 0;
    } else {
        this->m_cursor = std::max(std::min(this->m_cursor, (int)m_text.size()), 0);
    }
}
void TextField::draw(IO& io, int line) {
    int startPos = std::max(((m_cursor)-10), 0);
    io.writeString(m_text, -startPos, line);
    io.setCursor(m_cursor - startPos, line);
}
void TextField::input(IO::EInput input) {
    if (input == IO::EInput::left) {
        m_cursor--;
    }
    if (input == IO::EInput::right) {
        m_cursor++;
    }
    if (m_cursor < 0)
        m_cursor = m_text.size();
    if (m_cursor > m_text.size())
        m_cursor = 0;
    if (input == IO::EInput::back) {
        if (m_cursor > 0) {
            m_text.erase(m_cursor - 1, 1);
            m_cursor--;
        }
        return;
    }
    char ch = IO::decodeInput(input);
    if (ch != 0) {
        m_text.insert(m_cursor, 1, ch);
        m_cursor++;
    }
}
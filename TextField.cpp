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
void TextField::draw(IO& io, int line) {
    int startPos = std::max(m_cursor - 10, 0);
    io.writeString(m_text, startPos, line);
}
void TextField::input(IO::EInput input) {
    if (input == IO::EInput::left)
        m_cursor--;
    if (input == IO::EInput::right)
        m_cursor++;
    this->m_cursor = std::max(std::min(this->m_cursor, (int)m_text.size()), 0);
    char ch = IO::decodeInput(input);
    if (ch != 0) {
        m_text.insert(m_cursor, 1, ch);
        m_cursor++;
    }
}
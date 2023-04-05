#pragma once
#include <string>

#include "IO.hpp"

class TextField {
private:
    std::string m_text;
    int m_cursor;
    bool m_canEdit;
public:
    TextField() : TextField("") {}
    TextField(std::string text);
    ~TextField();
public:
    void setText(std::string text, bool resetCursor);
    int getCursor();
    std::string getText();
    void draw(IO& io, int line);
    void input(IO::EInput input);
    void setCanEdit(bool canEdit);
};
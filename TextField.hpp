#pragma once
#include <string>

#include "IO.hpp"

class TextField {
private:
    std::string m_text;
    int m_cursor;
public:
    TextField() : TextField("") {}
    TextField(std::string text);
    ~TextField();
public:
    int getCursor();
    std::string getText();
    void draw(IO& io, int line);
    void input(IO::EInput input);
};
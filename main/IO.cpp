#include "IO.hpp"

#include <stdexcept>

IO::IO() {}

IO::~IO() {}

char IO::decodeInput(IO::EInput input) {
    switch (input) {
        case EInput::n0:
            return '0';
        case EInput::n1:
            return '1';
        case EInput::n2:
            return '2';
        case EInput::n3:
            return '3';
        case EInput::n4:
            return '4';
        case EInput::n5:
            return '5';
        case EInput::n6:
            return '6';
        case EInput::n7:
            return '7';
        case EInput::n8:
            return '8';
        case EInput::n9:
            return '9';
        case EInput::p0:
            return 0x08;
        case EInput::p1:
            return 0x09;
        case EInput::p2:
            return 0x1e;
        case EInput::p3:
            return 0x1f;
        case EInput::p4:
            return 0x0a;
        case EInput::p5:
            return 0x0b;
        case EInput::p6:
            return 0x0c;
        case EInput::p7:
            return 0x0d;
        case EInput::p8:
            return 0x0e;
        case EInput::p9:
            return 0x0f;
        case EInput::ka:
            return 'a';
        case EInput::kb:
            return 'b';
        case EInput::kc:
            return 'c';
        case EInput::kd:
            return 'd';
        case EInput::ke:
            return 'e';
        case EInput::kf:
            return 'f';
        case EInput::kg:
            return 'g';
        case EInput::kh:
            return 'h';
        case EInput::ki:
            return 'i';
        case EInput::kj:
            return 'j';
        case EInput::kk:
            return 'k';
        case EInput::kl:
            return 'l';
        case EInput::km:
            return 'm';
        case EInput::kn:
            return 'n';
        case EInput::ko:
            return 'o';
        case EInput::kp:
            return 'p';
        case EInput::kr:
            return 'r';
        case EInput::ks:
            return 's';
        case EInput::kt:
            return 't';
        case EInput::ku:
            return 'u';
        case EInput::kv:
            return 'v';
        case EInput::kx:
            return 'x';
        case EInput::ky:
            return 'y';
        case EInput::kz:
            return 'z';
        case EInput::less:
            return '<';
        case EInput::more:
            return '>';
        case EInput::equals:
            return '=';
        case EInput::paranOpen:
            return '(';
        case EInput::paranClose:
            return ')';
        case EInput::plus:
            return '+';
        case EInput::minus:
            return '-';
        case EInput::times:
            return '*';
        case EInput::divide:
            return '/';
        case EInput::comma:
            return ',';
        case EInput::bang:
            return '!';
        default:
            return 0;
    }
}

void IO::writeString(std::string text, int x, int y) {
    if (y < 0 || y > 3)
        throw std::range_error("y out of bounds");
    for (int i = 0; i < text.size(); i++) {
        int xPos = x + i;
        if (xPos < 0 || xPos >= 20)
            continue;
        setChar(xPos, y, text.at(i));
    }
}
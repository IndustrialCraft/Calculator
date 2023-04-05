#pragma once

#include <string>

class IO {
public:
    enum EInput {
        none = 0,
        n0,
        n1,
        n2,
        n3,
        n4,
        n5,
        n6,
        n7,
        n8,
        n9,
        p0,
        p1,
        p2,
        p3,
        p4,
        p5,
        p6,
        p7,
        p8,
        p9,
        ka,
        kb,
        kc,
        kd,
        ke,
        kf,
        kg,
        kh,
        ki,
        kj,
        kk,
        kl,
        km,
        kn,
        ko,
        kp,
        kr,
        ks,
        kt,
        ku,
        kv,
        kx,
        ky,
        kz,
        less,
        more,
        equals,
        paranOpen,
        paranClose,
        plus,
        minus,
        times,
        divide,
        history,
        menu,
        exec,
        back,
        del,
        left,
        right,
        up,
        down,
        comma,
    };
public:
    IO();
    virtual ~IO();
public:
    virtual void setCursor(int x, int y) = 0;
    virtual void setCursorVisibility(bool visible) = 0;
    virtual void tick() = 0;
    virtual void setChar(int x, int y, char ch) = 0;
    void writeString(std::string text, int x, int y);
    virtual void flushDisplay() = 0;
    virtual EInput getInput() = 0;
public:
    static char decodeInput(EInput input);
};
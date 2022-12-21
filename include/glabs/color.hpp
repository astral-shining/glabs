#pragma once
#include <cpputils/types.hpp>

#ifndef _MATH_H
extern "C" double fmod(double, double);
extern "C" double fabs(double);
#endif

namespace GL {

struct RGBA {
    u8 r, g, b, a;
    constexpr RGBA(u8 r = 0, u8 g = 0, u8 b = 0, u8 a = 0) : r(r), g(g), b(b), a(a) {
    }
};

struct RGB {
    u8 r, g, b;
    constexpr RGB(u8 r = 0, u8 g = 0, u8 b = 0) : r(r), g(g), b(b) {}
};

constexpr RGBA HexRGBA(u32 num) {
    return RGBA(
        (num & 0xff000000) >> 24u, 
        (num & 0x00ff0000) >> 16u, 
        (num & 0x0000ff00) >> 8u, 
        (num & 0x000000ff)
    );
}

constexpr RGBA HSVA(float H, float S, float V, float A = 1.f) {
    float C = S * V; // Chroma
    float HPrime = fmod(H / 60, 6.f); // H'
    float X = C * (1 - fabs(fmod(HPrime, 2.f) - 1));
    float M = V - C;

    float R = 0.f;
    float G = 0.f;
    float B = 0.f;

    switch (static_cast<int>(HPrime)) {
    case 0: R = C; G = X;        break; // [0, 1)
    case 1: R = X; G = C;        break; // [1, 2)
    case 2:        G = C; B = X; break; // [2, 3)
    case 3:        G = X; B = C; break; // [3, 4)
    case 4: R = X;        B = C; break; // [4, 5)
    case 5: R = C;        B = X; break; // [5, 6)
    }

    R += M;
    G += M;
    B += M;

    RGBA color;
    color.r = (u8) (R * 255);
    color.g = (u8) (G * 255);
    color.b = (u8) (B * 255);
    color.a = (u8) (A * 255);
    return color;
}

};
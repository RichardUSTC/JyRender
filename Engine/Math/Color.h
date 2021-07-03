#pragma once
#include <cstdint>
#include <DirectXMath.h>

using ColorU32 = uint32_t; // ARGB

struct Color4B
{
    Color4B() = default;

    Color4B(ColorU32 color)
        : mA(static_cast<uint8_t>((color & 0xff000000) >> 24))
          , mR(static_cast<uint8_t>((color & 0xff0000) >> 16))
          , mG(static_cast<uint8_t>((color & 0xff00) >> 8))
          , mB(static_cast<uint8_t>((color & 0xff))) {}

    Color4B(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
        : mA(a)
          , mR(r)
          , mG(g)
          , mB(b) {}

    uint8_t mA{0};
    uint8_t mR{0};
    uint8_t mG{0};
    uint8_t mB{0};
};

struct Color3B
{
    Color3B() = default;

    Color3B(ColorU32 color)
        : mR(static_cast<uint8_t>((color & 0xff0000) >> 16))
          , mG(static_cast<uint8_t>((color & 0xff00) >> 8))
          , mB(static_cast<uint8_t>((color & 0xff))) {}

    Color3B(uint8_t r, uint8_t g, uint8_t b)
        : mR(r)
          , mG(g)
          , mB(b) {}

    uint8_t mR{0};
    uint8_t mG{0};
    uint8_t mB{0};
};

struct Color4F
{
    Color4F() = default;

    Color4F(float a, float r, float g, float b)
        : mA(a)
          , mR(r)
          , mG(g)
          , mB(b) {}

    float mA{0};
    float mR{0};
    float mG{0};
    float mB{0};
};

struct Color3F
{
    Color3F() = default;

    Color3F(float r, float g, float b)
        : mR(r)
          , mG(g)
          , mB(b) {}

    float mR{0};
    float mG{0};
    float mB{0};
};

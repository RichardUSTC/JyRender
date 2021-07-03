#pragma once

template <typename T>
struct RectTemplate
{
    RectTemplate() = default;

    RectTemplate(T left, T top, T width, T height)
        : mLeft(left)
          , mTop(top)
          , mWidth(width)
          , mHeight(height) {}

    T mLeft{0.0f};
    T mTop{0.0f};
    T mWidth{0.0f};
    T mHeight{0.0f};
};

using Rect = RectTemplate<uint32_t>;
using FRect = RectTemplate<float>;

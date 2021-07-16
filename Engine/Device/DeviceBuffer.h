#pragma once
#include "Common/Vector.h"

class Buffer
{
protected:
    uint32_t mBufferSize;
};

enum class ElementUsage
{
    Position,
    UV0,
    UV1,
    UV2,
    UV3,
    UV4,
    UV5,
    UV6,
    UV7,
    UV8,
    Normal,
    Tangent,
    VertexColor,
};

class VertexFormat
{
protected:
    Vector<ElementUsage> mUsages;
};

class VertexBuffer : public Buffer
{
protected:
    VertexFormat mFormat;
};

enum class IndexType
{
    Index8,
    Index16,
    Index32,
};

class IndexBuffer : public Buffer
{
protected:
    IndexType mIndexType{IndexType::Index16};
};

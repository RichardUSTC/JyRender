#pragma once
#include "Common/Ref.h"

#include <vector>

class Buffer : public Ref
{
protected:
    uint32_t mBufferSize{0};
};

const uint8_t MAX_BONE_COUNT      = 255;
const uint8_t MAX_BONE_PER_VERTEX = 4;
const uint8_t BONE_INDEX_SIZE     = sizeof(uint8_t);
const uint8_t BONE_WEIGHT_SIZE    = sizeof(float);

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
    Normal,
    Tangent,
    VertexColor,
    BoneIndices,
    BoneWeights,
};

inline const char* ATTRIBUTE_BINDING_DEFINES =
    "#define POSITION 0\n"
    "#define UV0 1\n"
    "#define UV1 2\n"
    "#define UV2 3\n"
    "#define UV3 4\n"
    "#define UV4 5\n"
    "#define UV5 6\n"
    "#define UV6 7\n"
    "#define UV7 8\n"
    "#define NORMAL 9\n"
    "#define TANGENT 10\n"
    "#define VERTEX_COLOR 11\n"
    "#define BONE_INDICES 12\n"
    "#define BONE_WEIGHTS 13\n";

enum class BufferUsage
{
    StaticDraw,
    StaticRead,
    StaticCopy,
    DynamicDraw,
    DynamicRead,
    DynamicCopy,
    StreamDraw,
    StreamRead,
    StreamCopy,
};

uint8_t GetElementUsageSize(ElementUsage usage);

struct VertexFormat
{
    void PushElement(ElementUsage usage);

    std::vector<ElementUsage> mElements;
    size_t                    mStride{0};
};

class VertexBuffer : public Buffer
{
public:
    const VertexFormat& GetFormat() const { return mFormat; }

protected:
    VertexFormat mFormat;
};

using VertexBufferPtr = OwnerPtr<VertexBuffer>;

enum class IndexType
{
    Index8,
    Index16,
    Index32,
};

inline size_t GetIndexSize(IndexType indexType)
{
    static const size_t IndexSizes[] = {1, 2, 4};
    return IndexSizes[static_cast<uint8_t>(indexType)];
}

class IndexBuffer : public Buffer
{
public:
    IndexType GetIndexType() const { return mIndexType; }

protected:
    IndexType mIndexType{IndexType::Index16};
};

using IndexBufferPtr = OwnerPtr<IndexBuffer>;

enum class PrimitiveType
{
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,
    TriangleFan,
};

struct Geometry
{
    std::vector<VertexBufferPtr> mVertexBuffers;
    IndexBufferPtr               mIndexBuffer{nullptr};
    size_t                       mVertexStart{0}; // Only used when mIndexBuffer is nullptr
    size_t                       mIndexStart{0};
    size_t                       mPrimitiveCount{0};
    PrimitiveType                mPrimitiveType{PrimitiveType::PointList};
};

#pragma once

#include "GLES/GLESDevice.h"
#include "DeviceBuffer.h"

class GLESVertexBuffer : public VertexBuffer
{
public:
    void Init(const VertexFormat& format, size_t    vertexCount,
              void*               data, BufferUsage bufferUsage);

    GLuint GetId() const { return mId; }

    void Destroy() override;

private:
    GLuint mId{0};
};

class GLESIndexBuffer : public IndexBuffer
{
public:
    void Init(IndexType indexType, size_t indexCount,
              void*     data, BufferUsage bufferUsage);

    GLuint GetId() const { return mId; }

    void Destroy() override;

private:
    GLuint mId{0};
};

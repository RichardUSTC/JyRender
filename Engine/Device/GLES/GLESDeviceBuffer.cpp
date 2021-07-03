#include "GLESDeviceBuffer.h"

inline GLenum GetGLBufferUsage(BufferUsage bufferUsage)
{
    static const GLenum BufferUsages[] =
    {
        GL_STATIC_DRAW,
        GL_STATIC_READ,
        GL_STATIC_COPY,
        GL_DYNAMIC_DRAW,
        GL_DYNAMIC_READ,
        GL_DYNAMIC_COPY,
        GL_STREAM_DRAW,
        GL_STREAM_READ,
        GL_STREAM_COPY,
    };
    return BufferUsages[static_cast<uint8_t>(bufferUsage)];
}

void GLESVertexBuffer::Init(const VertexFormat& format, size_t    vertexCount,
                            void*               data, BufferUsage bufferUsage)
{
    mFormat = format;
    glGenBuffers(1, &mId);
    if (!mId)
    {
        CHECK_GL_ERROR;
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, mId);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * format.mStride, data, GetGLBufferUsage((bufferUsage)));
}

void GLESVertexBuffer::Destroy()
{
    if (mId)
    {
        glDeleteBuffers(1, &mId);
        mId = 0;
    }
}

void GLESIndexBuffer::Init(IndexType indexType, size_t indexCount,
                           void*     data, BufferUsage bufferUsage)
{
    mIndexType = indexType;
    glGenBuffers(1, &mId);
    if (!mId)
    {
        CHECK_GL_ERROR;
        return;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexSize(indexType) * indexCount, data, GetGLBufferUsage(bufferUsage));
}

void GLESIndexBuffer::Destroy()
{
    if (mId)
    {
        glDeleteBuffers(1, &mId);
        mId = 0;
    }
}

#pragma once
#include "Device/Device.h"

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

class GLESDevice : public Device
{
public:
    void Init(const DeviceConfig& config) override;

    void Present() override;

    void Destroy() override;

    VertexBufferPtr CreateVertexBuffer(const VertexFormat& format, size_t    vertexCount,
                                       void*               data, BufferUsage bufferUsage) override;

    IndexBufferPtr CreateIndexBuffer(IndexType indexType, size_t indexCount,
                                     void*     data, BufferUsage bufferUsage) override;

    GraphicShaderPtr CreateGraphicShader(const std::string& vsContent, const std::string& fsContent,
                                         const std::string& macroDefines) override;

    void SetViewport(const Rect& viewport) override;

    void SetScissors(const Rect& scissors) override;

    void ApplyShader(GraphicShader* graphicShader) override;

    void DrawGeometry(const Geometry& geometry) override;

protected:
    void DoBeginPass() override;

    void DoClearColor(uint8_t colorIndex, Color4F clearColor) override;

    void DoClearDepth(float clearDepth) override;

    void DoClearStencil(uint8_t clearStencil) override;

    void DoEndPass() override;

private:
    EGLDisplay           mDisplay{nullptr};
    EGLSurface           mSurface{nullptr};
    EGLContext           mContext{nullptr};
    EGLNativeDisplayType mNativeDisplay{nullptr};
    GLuint               mFBO{0};
};

#if _DEBUG
#define CHECK_GL_ERROR do                                     \
{                                                             \
    GLenum error = glGetError();                              \
    if(error != GL_NO_ERROR)                                  \
        LOG_ERROR("Device", "glGetError: 0x{0:04x}", error);  \
} while(false)
#else
#define CHECK_GL_ERROR do {} while(false)
#endif

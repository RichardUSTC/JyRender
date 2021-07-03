#pragma once
#include "DeviceBuffer.h"
#include "DeviceShader.h"
#include "Common/Ref.h"
#include "Math/Color.h"
#include "Math/Vectors.h"

enum class DeviceType
{
    Unknown,
    OpenGLES3,
    DirectX11,
    Metal2
};

enum class ImageFormat
{
    RGBA8,
    D24,
    S8,
};

struct DeviceConfig
{
    DeviceType  mDeviceType{DeviceType::Unknown};
    void*       mNativeWindow{nullptr};
    ImageFormat mColorFormat{ImageFormat::RGBA8};
    ImageFormat mDepthFormat{ImageFormat::D24};
    ImageFormat mStencilFormat{ImageFormat::S8};
    uint8_t     mSampleCount{1};
};

const int MAX_COLOR_COUNT    = 16;
const int DEPTH_FLAG_INDEX   = MAX_COLOR_COUNT;
const int STENCIL_FLAG_INDEX = MAX_COLOR_COUNT + 1;
const int CLEAR_FLAGS_COUNT  = MAX_COLOR_COUNT + 2;

class Device : public Ref
{
public:
    virtual void Init(const DeviceConfig& config) = 0;

    virtual VertexBufferPtr CreateVertexBuffer(const VertexFormat& format, size_t    vertexCount,
                                               void*               data, BufferUsage bufferUsage) = 0;

    virtual IndexBufferPtr CreateIndexBuffer(IndexType indexType, size_t indexCount,
                                             void*     data, BufferUsage bufferUsage) = 0;

    virtual GraphicShaderPtr CreateGraphicShader(const std::string& vsContent, const std::string& fsContent,
                                                 const std::string& macroDefines) = 0;

    void BeginPass()
    {
        DoBeginPass();
    }

    void ClearColor(uint8_t colorIndex, Color4F clearColor)
    {
        DoClearColor(colorIndex, clearColor);
    }

    void SetClearDepth(float clearDepth)
    {
        DoClearDepth(clearDepth);
    }

    void SetClearStencil(uint8_t clearStencil)
    {
        DoClearStencil(clearStencil);
    }

    virtual void SetViewport(const Rect& viewport) = 0;

    virtual void SetScissors(const Rect& scissors) = 0;

    virtual void ApplyShader(GraphicShader* graphicShader) = 0;

    virtual void DrawGeometry(const Geometry& geometry) = 0;

    void EndPass() { DoEndPass(); }

    virtual void Present() = 0;

protected:
    virtual void DoEndPass() = 0;

    virtual void DoClearColor(uint8_t colorIndex, Color4F clearColor) = 0;

    virtual void DoClearDepth(float clearDepth) = 0;

    virtual void DoClearStencil(uint8_t clearStencil) = 0;

    virtual void DoBeginPass() = 0;
};

Device* CreateDevice(const DeviceConfig& config);

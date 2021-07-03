#include "GLESDevice.h"
#include "GLESDeviceBuffer.h"
#include "GLESDeviceShader.h"

#include "Application/Application.h"

#ifndef __APPLE__
///
// GetContextRenderableType()
//
//    Check whether EGL_KHR_create_context extension is supported.  If so,
//    return EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT
//
static EGLint GetContextRenderableType(EGLDisplay eglDisplay)
{
    #ifdef EGL_KHR_create_context
    // check whether EGL_KHR_create_context is in the extension string
    if (const char* extensions = eglQueryString(eglDisplay, EGL_EXTENSIONS); extensions != nullptr && strstr(
        extensions, "EGL_KHR_create_context"))
    {
        // extension is supported
        return EGL_OPENGL_ES3_BIT_KHR;
    }
    #endif

    // extension is not supported
    return EGL_OPENGL_ES2_BIT;
}
#endif

void GLESDevice::Init(const DeviceConfig& config)
{
    EGLConfig eglConfig;
    EGLint    majorVersion;
    EGLint    minorVersion;
    EGLint    contextAttributes[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};

    mDisplay = eglGetDisplay(mNativeDisplay);

    if (mDisplay == EGL_NO_DISPLAY)
    {
        LOG_CRITICAL("Device", "Failed to create EGLDisplay");
        return;
    }

    // Initialize EGL
    if (!eglInitialize(mDisplay, &majorVersion, &minorVersion))
    {
        LOG_CRITICAL("Device", "Failed to initialize EGL");
        return;
    }

    {
        int redSize = 8, greenSize = 8, blueSize = 8, alphaSize = 0, depthSize = 0, stencilSize = 0;
        switch (config.mColorFormat)
        {
        case ImageFormat::RGBA8:
            {
                redSize = greenSize = blueSize = 8;
                break;
            }
        default:
            {
                LOG_ERROR("Device", "ImageFormat {0} is not supported", config.mColorFormat);
                break;
            }
        }

        switch (config.mDepthFormat)
        {
        case ImageFormat::D24:
            {
                depthSize = 24;
                break;
            }
        default:
            {
                LOG_ERROR("Device", "ImageFormat {0} is not supported", config.mDepthFormat);
                break;
            }
        }

        switch (config.mStencilFormat)
        {
        case ImageFormat::S8:
            {
                stencilSize = 8;
                break;
            }
        default:
            {
                LOG_ERROR("Device", "ImageFormat {0} is not supported", config.mStencilFormat);
                break;
            }
        }

        EGLint numConfigs      = 0;
        EGLint attributeList[] =
        {
            EGL_RED_SIZE, redSize,
            EGL_GREEN_SIZE, greenSize,
            EGL_BLUE_SIZE, blueSize,
            EGL_ALPHA_SIZE, alphaSize ? alphaSize : EGL_DONT_CARE,
            EGL_DEPTH_SIZE, depthSize ? depthSize : EGL_DONT_CARE,
            EGL_STENCIL_SIZE, stencilSize ? stencilSize : EGL_DONT_CARE,
            EGL_SAMPLE_BUFFERS, config.mSampleCount > 1 ? 1 : 0,
            // if EGL_KHR_create_context extension is supported, then we will use
            // EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
            EGL_RENDERABLE_TYPE, GetContextRenderableType(mDisplay),
            EGL_NONE
        };

        // Choose config
        if (!eglChooseConfig(mDisplay, attributeList, &eglConfig, 1, &numConfigs))
        {
            LOG_CRITICAL("Device", "Failed to choose EGL config");
            return;
        }

        if (numConfigs < 1)
        {
            LOG_CRITICAL("Device", "No available config");
            return;
        }
    }

    #ifdef ANDROID
    // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
    {
        EGLint format = 0;
        eglGetConfigAttrib(esContext->eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(esContext->eglNativeWindow, 0, 0, format);
    }
    #endif // ANDROID

    // Create a surface
    auto nativeWindow = static_cast<EGLNativeWindowType>(config.mNativeWindow);
    mSurface          = eglCreateWindowSurface(mDisplay, eglConfig, nativeWindow, nullptr);

    if (mSurface == EGL_NO_SURFACE)
    {
        LOG_CRITICAL("Device", "Failed to create surface");
        return;
    }

    // Create a GL context
    mContext = eglCreateContext(mDisplay, eglConfig,
                                EGL_NO_CONTEXT, contextAttributes);

    if (mContext == EGL_NO_CONTEXT)
    {
        LOG_CRITICAL("Device", "Failed to create context");
        return;
    }

    // Make the context current
    if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext))
    {
        LOG_CRITICAL("Device", "Failed to MakeCurrent");
    }
}

void GLESDevice::Present()
{
    eglSwapBuffers(mDisplay, mSurface);
}

void GLESDevice::Destroy()
{
    if (mContext)
    {
        eglDestroyContext(mDisplay, mContext);
        mContext = nullptr;
    }
    if (mSurface)
    {
        eglDestroySurface(mDisplay, mSurface);
        mSurface = nullptr;
    }
}

VertexBufferPtr GLESDevice::CreateVertexBuffer(const VertexFormat& format, size_t    vertexCount,
                                               void*               data, BufferUsage bufferUsage)
{
    auto* vertexBuffer = new GLESVertexBuffer;
    vertexBuffer->Init(format, vertexCount, data, bufferUsage);
    return vertexBuffer;
}

IndexBufferPtr GLESDevice::CreateIndexBuffer(IndexType indexType, size_t indexCount,
                                             void*     data, BufferUsage bufferUsage)
{
    auto* indexBuffer = new GLESIndexBuffer;
    indexBuffer->Init(indexType, indexCount, data, bufferUsage);
    return indexBuffer;
}

GraphicShaderPtr GLESDevice::CreateGraphicShader(const std::string& vsContent, const std::string& fsContent,
                                                 const std::string& macroDefines)
{
    auto* shader = new GLESGraphicShader();
    shader->Init(vsContent, fsContent, macroDefines);
    return shader;
}

void GLESDevice::DoBeginPass() { }

void GLESDevice::DoClearColor(uint8_t colorIndex, Color4F clearColor)
{
    GLfloat color[] = {clearColor.mR, clearColor.mG, clearColor.mB, clearColor.mA};
    glClearBufferfv(GL_COLOR, colorIndex, color);
    CHECK_GL_ERROR;
}

void GLESDevice::DoClearDepth(float clearDepth)
{
    glClearBufferfv(GL_DEPTH, 0, &clearDepth);
    CHECK_GL_ERROR;
}

void GLESDevice::DoClearStencil(uint8_t clearStencil)
{
    GLint stencil = clearStencil;
    glClearBufferiv(GL_STENCIL, 0, &stencil);
    CHECK_GL_ERROR;
}

void GLESDevice::SetViewport(const Rect& viewport)
{
    glViewport(viewport.mLeft, viewport.mTop, viewport.mWidth, viewport.mHeight);
    CHECK_GL_ERROR;
}

void GLESDevice::SetScissors(const Rect& scissors)
{
    glScissor(scissors.mLeft, scissors.mTop, scissors.mWidth, scissors.mHeight);
    CHECK_GL_ERROR;
}

void GLESDevice::ApplyShader(GraphicShader* graphicShader)
{
    // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    auto shader = static_cast<GLESGraphicShader*>(graphicShader);
    glUseProgram(shader->mProgram);
    CHECK_GL_ERROR;
}

static GLuint GetElementUsageComponentCount(ElementUsage usage)
{
    switch (usage)
    {
    case ElementUsage::Position:
        return 3;
    case ElementUsage::UV0:
    case ElementUsage::UV1:
    case ElementUsage::UV2:
    case ElementUsage::UV3:
    case ElementUsage::UV4:
    case ElementUsage::UV5:
    case ElementUsage::UV6:
    case ElementUsage::UV7:
        return 2;
    case ElementUsage::Normal:
    case ElementUsage::Tangent:
        return 3;
    case ElementUsage::VertexColor:
        return 4;
    case ElementUsage::BoneIndices:
        return MAX_BONE_PER_VERTEX;
    case ElementUsage::BoneWeights:
        return MAX_BONE_PER_VERTEX;
    default: return 0;
    }
}

static GLenum GetElementUsageType(ElementUsage usage)
{
    switch (usage)
    {
    case ElementUsage::Position:
    case ElementUsage::UV0:
    case ElementUsage::UV1:
    case ElementUsage::UV2:
    case ElementUsage::UV3:
    case ElementUsage::UV4:
    case ElementUsage::UV5:
    case ElementUsage::UV6:
    case ElementUsage::UV7:
    case ElementUsage::Normal:
    case ElementUsage::Tangent:
        return GL_FLOAT;
    case ElementUsage::VertexColor:
        return GL_UNSIGNED_BYTE;
    case ElementUsage::BoneIndices:
        {
            static const GLenum sizeList[] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT};
            return sizeList[BONE_INDEX_SIZE - 1];
        }
    case ElementUsage::BoneWeights:
        return GL_FLOAT;
    default: return GL_BYTE;
    }
}

static GLboolean GetComponentUsageNormalized(ElementUsage usage)
{
    if (usage == ElementUsage::VertexColor)
        return GL_TRUE;
    return GL_FALSE;
}

static GLenum GetDrawMode(PrimitiveType primitiveType)
{
    switch (primitiveType)
    {
    case PrimitiveType::PointList: return GL_POINTS;
    case PrimitiveType::LineList: return GL_LINES;
    case PrimitiveType::LineStrip: return GL_LINE_STRIP;
    case PrimitiveType::TriangleList: return GL_TRIANGLES;
    case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
    case PrimitiveType::TriangleFan: return GL_TRIANGLE_FAN;
    default: return GL_POINTS;
    }
}

static GLsizei GetDrawCount(PrimitiveType primitiveType, size_t primitiveCount)
{
    switch (primitiveType)
    {
    case PrimitiveType::PointList: return static_cast<GLsizei>(primitiveCount);
    case PrimitiveType::LineList: return static_cast<GLsizei>(primitiveCount * 2);
    case PrimitiveType::LineStrip: return static_cast<GLsizei>(primitiveCount + 1);
    case PrimitiveType::TriangleList: return static_cast<GLsizei>(primitiveCount * 3);
    case PrimitiveType::TriangleStrip:
    case PrimitiveType::TriangleFan: return static_cast<GLsizei>(primitiveCount + 2);
    default: return 0;
    }
}

static GLenum GetIndexType(IndexType indexType)
{
    switch (indexType)
    {
    case IndexType::Index8: return GL_UNSIGNED_BYTE;
    case IndexType::Index16: return GL_UNSIGNED_SHORT;
    case IndexType::Index32: return GL_UNSIGNED_INT;
    default: return GL_UNSIGNED_SHORT;
    }
}

void GLESDevice::DrawGeometry(const Geometry& geometry)
{
    for (auto& vb : geometry.mVertexBuffers)
    {
        auto vertexBuffer = static_cast<const GLESVertexBuffer*>(vb.Get());
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetId());

        GLuint   stride = static_cast<GLuint>(vertexBuffer->GetFormat().mStride);
        uint64_t offset = 0;
        for (auto elementUsage : vertexBuffer->GetFormat().mElements)
        {
            glEnableVertexAttribArray(static_cast<GLuint>(elementUsage));
            CHECK_GL_ERROR;
            glVertexAttribPointer(static_cast<GLuint>(elementUsage),
                                  GetElementUsageComponentCount(elementUsage),
                                  GetElementUsageType(elementUsage),
                                  GetComponentUsageNormalized(elementUsage),
                                  stride,
                                  reinterpret_cast<const void*>(offset));
            CHECK_GL_ERROR;
            offset += GetElementUsageSize(elementUsage);
        }
    }

    GLenum  drawMode  = GetDrawMode(geometry.mPrimitiveType);
    GLsizei drawCount = GetDrawCount(geometry.mPrimitiveType, geometry.mPrimitiveCount);

    if (geometry.mIndexBuffer.Get())
    {
        auto indexBuffer = static_cast<const GLESIndexBuffer*>(geometry.mIndexBuffer.Get());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetId());
        IndexType indexType = indexBuffer->GetIndexType();
        glDrawElements(drawMode,
                       drawCount,
                       GetIndexType(indexType),
                       reinterpret_cast<const void*>(geometry.mIndexStart * GetIndexSize(indexType)));
        CHECK_GL_ERROR;
    }
    else
    {
        glDrawArrays(drawMode, static_cast<GLint>(geometry.mVertexStart), drawCount);
        CHECK_GL_ERROR;
    }
}

void GLESDevice::DoEndPass() {}

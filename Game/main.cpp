#include <cstdio>
#include "Engine.h"

struct UserData
{
    GraphicShaderPtr mShader;
    Geometry         mGeometry;
};

static void Initialize(UserData* userData)
{
    char vShaderStr[] =
        "layout(location = POSITION) in vec4 vPosition;  \n"
        "void main()                                     \n"
        "{                                               \n"
        "   gl_Position = vPosition;                     \n"
        "}                                               \n";

    char fShaderStr[] =
        "precision mediump float;                     \n"
        "out vec4 fragColor;                          \n"
        "void main()                                  \n"
        "{                                            \n"
        "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
        "}                                            \n";

    auto* app         = Application::GetInstance();
    auto  device      = app->GetDevice();
    userData->mShader = device->CreateGraphicShader(vShaderStr, fShaderStr, "");

    float vVertices[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    uint16_t vIndices[] = {
        0, 1, 2
    };

    VertexFormat format;
    format.PushElement(ElementUsage::Position);
    auto vertexBuffer = device->CreateVertexBuffer(format, 3, vVertices, BufferUsage::StaticDraw);
    userData->mGeometry.mVertexBuffers.push_back(vertexBuffer);

    auto indexBuffer = device->CreateIndexBuffer(IndexType::Index16, 3, vIndices, BufferUsage::StaticDraw);
    userData->mGeometry.mIndexBuffer = indexBuffer;

    userData->mGeometry.mPrimitiveType  = PrimitiveType::TriangleList;
    userData->mGeometry.mPrimitiveCount = 1;
}

static void Draw(UserData* userData)
{
    auto* app    = Application::GetInstance();
    auto  device = app->GetDevice();

    device->BeginPass();
    uint32_t width  = Application::GetInstance()->GetWidth();
    uint32_t height = Application::GetInstance()->GetHeight();
    Rect     frameSize(0, 0, width, height);

    device->SetViewport(frameSize);
    device->SetScissors(frameSize);

    device->ClearColor(0, Color4F(1.0f, 0.0f, 1.0f, 0.0f));

    device->ApplyShader(userData->mShader.Get());
    device->DrawGeometry(userData->mGeometry);

    device->EndPass();
}

static void Cleanup(UserData* userData)
{
    userData->mGeometry.mVertexBuffers.clear();
    userData->mGeometry.mIndexBuffer.Reset();
    userData->mShader.Reset();
}

int main()
{
    UserData userData;

    Application       app;
    ApplicationConfig config;

    config.mWidth           = 640;
    config.mHeight          = 360;
    config.mLogicFrameRate  = 60;
    config.mRenderFrameRate = 60;
    config.mDeviceType      = DeviceType::OpenGLES3;

    config.mInitCallback = [&]()
    {
        Initialize(&userData);
    };

    config.mUpdateCallback = [&]()
    {
        Draw(&userData);
    };

    config.mExitCallback = [&]()
    {
        Cleanup(&userData);
    };

    app.Init(config);
    app.Loop();

    return 0;
}

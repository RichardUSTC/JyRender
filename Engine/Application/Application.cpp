#include "Application.h"
#include "Log/Log.h"

Application* Application::sInstance = nullptr;

Application::Application()
{
    assert(sInstance == nullptr);
    sInstance = this;
}

void Application::Init(const ApplicationConfig& config)
{
    mWidth           = config.mWidth;
    mHeight          = config.mHeight;
    mLogicFrameRate  = config.mLogicFrameRate > 0 ? config.mLogicFrameRate : mLogicFrameRate;
    mRenderFrameRate = config.mRenderFrameRate > 0 ? config.mRenderFrameRate : mRenderFrameRate;
    mUpdateCallback  = config.mUpdateCallback;
    mExitCallback    = config.mExitCallback;

    // Init window
    mWindow = new Window;
    WindowConfig windowConfig;
    windowConfig.mWidth  = config.mWidth;
    windowConfig.mHeight = config.mHeight;
    windowConfig.mLeft   = 100;
    windowConfig.mTop    = 100;
    windowConfig.mTitle  = "JyRender";
    mWindow.Get()->Init(windowConfig);

    // Init device
    DeviceConfig deviceConfig;
    deviceConfig.mDeviceType    = config.mDeviceType;
    deviceConfig.mNativeWindow  = mWindow.Get()->GetNativeWindow();
    deviceConfig.mColorFormat   = ImageFormat::RGBA8;
    deviceConfig.mDepthFormat   = ImageFormat::D24;
    deviceConfig.mStencilFormat = ImageFormat::S8;
    deviceConfig.mSampleCount   = 1;
    mDevice                     = CreateDevice(deviceConfig);

    if (config.mInitCallback)
        config.mInitCallback();
}

void Application::Loop()
{
    LOG_ASSERT_MSG(mUpdateCallback, "Application", "Application::mUpdateCallback is not set");
    LOG_ASSERT_MSG(mWindow.Get(), "Application", "Application::mWindow is not initialized");

    const Duration frameDuration = FrameDuration(mLogicFrameRate);
    while (mRunning)
    {
        mFrameStartTime = GetNow();

        mWindow.Get()->OnUpdate();
        mUpdateCallback();

        mDevice.Get()->Present();

        SleepUntil(mFrameStartTime + frameDuration);
        mFrameCount++;
    }

    if (mExitCallback)
        mExitCallback();
}

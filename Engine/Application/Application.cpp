#include "Application.h"
#include "Log/Log.h"
#include <cassert>

Application* Application::sInstance = nullptr;

Application::Application()
{
    assert(sInstance == nullptr);
    sInstance = this;
}

void Application::Init(const ApplicationConfig& config)
{
    mWidth            = config.mWidth;
    mHeight           = config.mHeight;
    mLogicFrameRate   = config.mLogicFrameRate > 0 ? config.mLogicFrameRate : mLogicFrameRate;
    mRenderFrameRate  = config.mRenderFrameRate > 0 ? config.mRenderFrameRate : mRenderFrameRate;
    mOnUpdateCallback = config.mOnUpdateCallback;

    if (config.mOnInitCallback)
    {
        config.mOnInitCallback(this);
    }
}

void Application::Loop()
{
    LOG_ASSERT_MSG(mOnUpdateCallback, "Application", "Application::mOnUpdateCallback is not set");
    LOG_ASSERT_MSG(mWindow.Get(), "Application", "Application::mWindow is not initialized");
	
    const Duration frameDuration = FrameDuration(mLogicFrameRate);
    while (mRunning)
    {
        mFrameStartTime = GetNow();

        mWindow.Get()->OnUpdate();
        mOnUpdateCallback();

        SleepUntil(mFrameStartTime + frameDuration);
        mFrameCount++;
    }
}

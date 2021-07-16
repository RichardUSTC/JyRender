#include "Application.h"
#include "Common/Time.h"
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
    const Duration frameDuration = FrameDuration(mLogicFrameRate);
    while (mRunning)
    {
        TimePoint now = GetNow();
        if (mOnUpdateCallback)
            mOnUpdateCallback();

        SleepUntil(now + frameDuration);
        mFrameCount++;
    }
}

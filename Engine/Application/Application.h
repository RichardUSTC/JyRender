#pragma once
#include <cstdint>
#include "Window.h"
#include "Common/TimeUtils.h"

class Application;

using OnInitCallback = void(*)(Application* app);
using OnUpdateCallback = void(*)();

struct ApplicationConfig
{
    uint32_t         mWidth{640};
    uint32_t         mHeight{360};
    uint32_t         mLogicFrameRate{60};
    uint32_t         mRenderFrameRate{60};
    OnInitCallback   mOnInitCallback{nullptr};
    OnUpdateCallback mOnUpdateCallback{nullptr};
};

class Application
{
public:
    Application();

    void Init(const ApplicationConfig& config);

    void Loop();

    static Application* GetInstance() { return sInstance; }

    uint64_t GetFrameCount() const { return mFrameCount; }

    Window* GetWindow() { return mWindow.Get(); }

    void Exit() { mRunning = false; }

private:
    bool             mRunning{true};
    uint32_t         mWidth{640};
    uint32_t         mHeight{360};
    uint32_t         mLogicFrameRate{60};
    uint32_t         mRenderFrameRate{60};
    uint64_t         mFrameCount{0};
    TimePoint        mFrameStartTime;
    OnUpdateCallback mOnUpdateCallback{nullptr};
    OwnerPtr<Window> mWindow;
    
    static Application* sInstance;
};

#pragma once
#include <cstdint>
#include <functional>
#include "Window.h"
#include "Common/TimeUtils.h"
#include "Device/Device.h"

class Application;

using InitCallback = std::function<void()>;
using UpdateCallback = std::function<void()>;
using ExitCallback = std::function<void()>;

struct ApplicationConfig
{
    uint32_t       mWidth{640};
    uint32_t       mHeight{360};
    uint32_t       mLogicFrameRate{60};
    uint32_t       mRenderFrameRate{60};
    DeviceType     mDeviceType{DeviceType::Unknown};
    InitCallback   mInitCallback{nullptr};
    UpdateCallback mUpdateCallback{nullptr};
    ExitCallback   mExitCallback{nullptr};
};

class Application
{
public:
    Application();

    void Init(const ApplicationConfig& config);

    void Loop();

    static Application* GetInstance() { return sInstance; }

    uint64_t GetFrameCount() const { return mFrameCount; }

    uint32_t GetWidth() const { return mWidth; }

    uint32_t GetHeight() const { return mHeight; }

    Window* GetWindow() { return mWindow.Get(); }

    Device* GetDevice() { return mDevice.Get(); }

    void Exit() { mRunning = false; }

private:
    bool             mRunning{true};
    uint32_t         mWidth{640};
    uint32_t         mHeight{360};
    uint32_t         mLogicFrameRate{60};
    uint32_t         mRenderFrameRate{60};
    uint64_t         mFrameCount{0};
    TimePoint        mFrameStartTime;
    UpdateCallback   mUpdateCallback{nullptr};
    ExitCallback     mExitCallback{nullptr};
    OwnerPtr<Window> mWindow;
    OwnerPtr<Device> mDevice;

    static Application* sInstance;
};

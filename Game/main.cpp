#include <cstdio>
#include "Application/Application.h"
#include "Log/Log.h"

int main()
{
    Application       app;
    ApplicationConfig config;

    config.mWidth           = 640;
    config.mHeight          = 360;
    config.mLogicFrameRate  = 60;
    config.mRenderFrameRate = 60;
    config.mOnInitCallback  = [](Application* appPtr)
    {
        LOG_INFO("Game", "Init {0}", fmt::ptr(appPtr));
    };
    config.mOnUpdateCallback = []()
    {
        if (Application::GetInstance()->GetFrameCount() % 60 == 0)
            LOG_INFO("Game", "Loop");
    };

    app.Init(config);
    app.Loop();

    return 0;
}

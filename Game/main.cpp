#include <cstdio>
#include "Application/Application.h"

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
        printf("init %p\n", appPtr);
    };
    config.mOnUpdateCallback = []()
    {
        printf("loop\n");
    };

    app.Init(config);
    app.Loop();

    printf("hello, world\n");
    return 0;
}

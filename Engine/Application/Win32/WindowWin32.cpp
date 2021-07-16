#include "Window.h"
#include "Log/Log.h"
#include "Application.h"

#define UNICODE

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

LRESULT WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = 1;
    switch (message)
    {
    case WM_CREATE:
    {
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_CHAR:
    {
        LOG_INFO("Window", "Input char %s", (unsigned char)wParam);
        break;
    }
    default:
    {
        ret = DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    }
    return ret;
}

void* CreateNativeWindow(const WindowConfig& config)
{
    LOG_INFO("Window", "CreateWindow: %s %d %d %d %d", config.mTitle, config.mLeft, config.mTop, config.mWidth, config.mHeight);


    WNDCLASS  windowClass = { 0 };
    RECT      windowRect;
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    windowClass.style = CS_OWNDC;
    windowClass.lpfnWndProc = (WNDPROC)WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.lpszClassName = L"JyRender";

    if (!RegisterClass(&windowClass)) {
        LOG_CRITICAL("Window", "RegisterClass failed");
        return nullptr;
    }

    DWORD wStyle      = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;
    windowRect.left   = config.mLeft;
    windowRect.top    = config.mTop;
    windowRect.right  = config.mWidth;
    windowRect.bottom = config.mHeight;
    AdjustWindowRect(&windowRect, wStyle, FALSE);

    size_t titleLength = strlen(config.mTitle);
    auto* title = new WCHAR[titleLength + 1];
    MultiByteToWideChar(CP_UTF8, 0, config.mTitle, (int)(titleLength + 1), title, (int)(titleLength + 1));
	
    auto nativeWindow = CreateWindow(L"JyRender", title, wStyle, 0, 0,
        config.mWidth,
        config.mHeight, nullptr, nullptr,
        hInstance, nullptr);

    delete title;
	
    if (!nativeWindow) {
        LOG_CRITICAL("Window", "CreateWindow failed");
        return nullptr;
    }

    ShowWindow(nativeWindow, TRUE);
    return nativeWindow;
}

void NativeWindowOnUpdate(void* nativeWindow)
{
    (void)nativeWindow;
	
    MSG message;
    while (!(PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)))
    {
        if (message.message == WM_QUIT)
        {
            Application::GetInstance()->Exit();
        }
        else
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
}

void DestroyNativeWindow(void* nativeWindow)
{
	if(!DestroyWindow(static_cast<HWND>(nativeWindow)))
	{
        LOG_CRITICAL("Window", "Failed to destroy window");
	}
}

#include "Window.h"
#include "Log/Log.h"
#include "Application.h"

#define UNICODE

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

static LRESULT WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        break;

    case WM_CLOSE:
        {
            LOG_INFO("Window", "WM_CLOSE");
            Application::GetInstance()->Exit();
            return 0;
        }

    case WM_CHAR:
        {
            LOG_INFO("Window", "WM_CHAR {0}", static_cast<unsigned char>(wParam));
            break;
        }

    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void* CreateNativeWindow(const WindowConfig& config)
{
    LOG_INFO("Window", "CreateWindow: {0} {1} {2} {3} {4}", config.mTitle, config.mLeft, config.mTop,
             config.mWidth, config.mHeight);

    WNDCLASS  wndclass = {0};
    DWORD     wStyle   = 0;
    RECT      windowRect;
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    wndclass.style         = CS_OWNDC;
    wndclass.lpfnWndProc   = static_cast<WNDPROC>(WindowProc);
    wndclass.hInstance     = hInstance;
    wndclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wndclass.lpszClassName = L"JyRender";

    if (!RegisterClass(&wndclass))
    {
        LOG_CRITICAL("Window", "RegisterClass failed");
        return nullptr;
    }

    wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

    // Adjust the window rectangle so that the client area has
    // the correct number of pixels
    windowRect.left   = 0;
    windowRect.top    = 0;
    windowRect.right  = config.mWidth;
    windowRect.bottom = config.mHeight;

    AdjustWindowRect(&windowRect, wStyle, FALSE);

    size_t titleLength = strlen(config.mTitle);
    auto*  title       = new WCHAR[titleLength + 1];
    MultiByteToWideChar(CP_UTF8, 0, config.mTitle, static_cast<int>(titleLength + 1), title,
                        static_cast<int>(titleLength + 1));

    auto nativeWindow = CreateWindow(
        L"JyRender",
        title,
        wStyle,
        0,
        0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    delete[] title;

    if (!nativeWindow)
    {
        LOG_CRITICAL("Window", "CreateWindow failed");
        return nullptr;
    }

    ShowWindow(nativeWindow, TRUE);
    SetWindowPos(nativeWindow, HWND_TOP, config.mLeft, config.mTop, 0, 0, SWP_NOSIZE);

    return nativeWindow;
}

void NativeWindowOnUpdate(void* nativeWindow)
{
    (void)nativeWindow;

    MSG message;
    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
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
    if (!DestroyWindow(static_cast<HWND>(nativeWindow)))
    {
        LOG_CRITICAL("Window", "Failed to destroy window");
    }
}

#include "Window.h"
#include "Log/Log.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

LRESULT WINAPI WindowProc(HWND hWnd, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT ret = 1;
    switch (message)
    {
    case WM_CREATE:
    {
        break;
    }
    case WM_PAINT:
    {
        // Logger *logger = Logger::get_logger("Render");
        // logger->info("WM_PAINT");
        Device* device = Device::get_device();
        Renderer* renderer = device->get_renderer();
        renderer->draw();
        ValidateRect(device->get_native_window(), nullptr);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_CHAR:
    {
        Logger* logger = Logger::get_logger("Input");
        logger->info("WM_CHAR");
        break;
    }
    default:
    {
        ret = DefWindowProc(hWnd, message, w_param, l_param);
        break;
    }
    }
    return ret;
}

uint64_t CreateNativeWindow(const WindowConfig& config)
{
    LOG_INFO("Window", "CreateWindow: %s %d %d %d %d", config.mTitle, config.mLeft, config.mTop, config.mWidth, config.mHeight);


    WNDCLASS  windowClass = { 0 };
    RECT      windowRect;
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    windowClass.style = CS_OWNDC;
    windowClass.lpfnWndProc = (WNDPROC)WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.lpszClassName = "JyRender";

    if (!RegisterClass(&windowClass)) {
        LOG_CRITICAL("Window", "RegisterClass failed");
        return 0;
    }

    DWORD wStyle      = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;
    windowRect.left   = config.mLeft;
    windowRect.top    = config.mTop;
    windowRect.right  = config.mWidth;
    windowRect.bottom = config.mHeight;
    AdjustWindowRect(&windowRect, wStyle, FALSE);

    auto nativeWindow = CreateWindow("JyRender", config.mTitle, wStyle, 0, 0,
        config.mWidth,
        config.mHeight, nullptr, nullptr,
        hInstance, nullptr);

    if (!nativeWindow) {
        LOG_CRITICAL("Window", "CreateWindow failed");
        return 0;
    }

    ShowWindow(nativeWindow, TRUE);
    return reinterpret_cast<uint64_t>(nativeWindow);
}

void DestroyNativeWindow(uint64_t nativeWindow)
{
	if(!DestroyWindow(reinterpret_cast<HWND>(nativeWindow)))
	{
        LOG_CRITICAL("Window", "Failed to destroy window");
	}
}

void Device::win_loop() {
  MSG msg = {0};
  int done = 0;
  DWORD last_time = GetTickCount();

  while (!done) {
    int got_msg = !(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE));
    DWORD cur_time = GetTickCount();
    float delta_time = (cur_time - last_time) / 1000.0f;
    last_time = cur_time;

    if (got_msg) {
      if (msg.message == WM_QUIT) {
        done = 1;
      } else {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    } else {
      SendMessage(_window, WM_PAINT, 0, 0);
    }

    // device update
    update(delta_time);
  }
}
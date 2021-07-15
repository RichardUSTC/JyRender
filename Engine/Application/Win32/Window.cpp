#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

LRESULT WINAPI window_proc(HWND h_wnd, UINT u_msg, WPARAM w_param,
                           LPARAM l_param);

bool Device::_create_native_window(const char *title, uint32_t width,
                                   uint32_t height) {
  Logger *logger = Logger::get_logger("Device");
  logger->info("create window: %s", title);

  WNDCLASS wndclass = {0};
  DWORD w_style = 0;
  RECT window_rect;
  HINSTANCE h_instance = GetModuleHandle(nullptr);

  wndclass.style = CS_OWNDC;
  wndclass.lpfnWndProc = (WNDPROC)window_proc;
  wndclass.hInstance = h_instance;
  wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wndclass.lpszClassName = "JyRender";

  if (!RegisterClass(&wndclass)) {
    logger->error("RegisterClass failed");
    return false;
  }

  w_style = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;
  window_rect.left = 0;
  window_rect.top = 0;
  window_rect.right = width;
  window_rect.bottom = height;
  AdjustWindowRect(&window_rect, w_style, FALSE);

  _window = CreateWindow("JyRender", title, w_style, 0, 0,
                         window_rect.right - window_rect.left,
                         window_rect.bottom - window_rect.top, nullptr, nullptr,
                         h_instance, nullptr);

  if (!_window) {
    logger->error("CreateWindow failed");
    return false;
  }

  ShowWindow(_window, TRUE);

  return true;
}

LRESULT WINAPI window_proc(HWND h_wnd, UINT u_msg, WPARAM w_param,
                           LPARAM l_param) {
  LRESULT ret = 1;
  switch (u_msg) {
  case WM_CREATE: {
    break;
  }
  case WM_PAINT: {
    // Logger *logger = Logger::get_logger("Render");
    // logger->info("WM_PAINT");
    Device *device = Device::get_device();
    Renderer *renderer = device->get_renderer();
    renderer->draw();
    ValidateRect(device->get_native_window(), nullptr);
    break;
  }
  case WM_DESTROY: {
    PostQuitMessage(0);
    break;
  }
  case WM_CHAR: {
    Logger *logger = Logger::get_logger("Input");
    logger->info("WM_CHAR");
    break;
  }
  default: {
    ret = DefWindowProc(h_wnd, u_msg, w_param, l_param);
    break;
  }
  }
  return ret;
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
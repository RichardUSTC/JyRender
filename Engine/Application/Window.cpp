#include "Window.h"
#include "Log/Log.h"

uint64_t CreateNativeWindow(const WindowConfig& config);
void DestroyNativeWindow(uint64_t nativeWindow);

void Window::Init(const WindowConfig& config)
{
	LOG_ASSERT_MSG(!mNativeWindow, "Window", "Window should only init once");
	mNativeWindow = CreateNativeWindow(config);
}

Window::~Window()
{
	if (mNativeWindow)
		DestroyNativeWindow(mNativeWindow);
}

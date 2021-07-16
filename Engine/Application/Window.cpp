#include "Window.h"
#include "Log/Log.h"

void* CreateNativeWindow(const WindowConfig& config);
void NativeWindowOnUpdate(void* nativeWindow);
void DestroyNativeWindow(void* nativeWindow);

void Window::Init(const WindowConfig& config)
{
	LOG_ASSERT_MSG(!mNativeWindow, "Window", "Window should only init once");
	mNativeWindow = CreateNativeWindow(config);
}

void Window::OnUpdate()
{
	LOG_ASSERT_MSG(mNativeWindow, "Window", "Window::mNativeWindow is not initialized");
	NativeWindowOnUpdate(mNativeWindow);
}

void Window::Destroy()
{
	if (mNativeWindow)
	{
		DestroyNativeWindow(mNativeWindow);
		mNativeWindow = nullptr;
	}
}

#pragma once
#include "Common/Ref.h"

struct WindowConfig
{
	uint32_t    mLeft{ 0 };
	uint32_t    mTop{ 0 };
	uint32_t    mWidth{ 128 };
	uint32_t    mHeight{ 128 };
	const char* mTitle{ "" };
};

class Window: public Ref
{
public:
	void Init(const WindowConfig& config);

	void OnUpdate();

	void* GetNativeWindow() const { return mNativeWindow; }
	
	void Destroy() override;
private:
	void* mNativeWindow {nullptr};
};

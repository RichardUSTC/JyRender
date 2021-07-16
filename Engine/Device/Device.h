#pragma once
#include "Common/Ref.h"

enum class DeviceType
{
    Unknown,
    OpenGLES3,
    DirectX11,
    Metal2
};

struct DeviceConfig
{
    uint32_t mWidth{128};
    uint32_t mHeight{128};
};

class Device : Ref
{
public:
    void Init(const DeviceConfig& config);

    void Destroy() override;

protected:
    virtual void DoInit(const DeviceConfig& config) = 0;

    virtual void DoDestroy() = 0;

    DeviceType mDeviceType{DeviceType::Unknown};
};

#include "Device.h"

void Device::Init(const DeviceConfig& config)
{
    DoInit(config);
}

void Device::Destroy()
{
    DoDestroy();
}

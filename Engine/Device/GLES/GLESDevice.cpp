#include "GLESDevice.h"

void GLESDevice::DoInit(const DeviceConfig& config)
{
    mDeviceType = DeviceType::OpenGLES3;
}

void GLESDevice::DoDestroy()
{
    // TODO
}

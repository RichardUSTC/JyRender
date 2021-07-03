#include "Device.h"
#include "GLES/GLESDevice.h"

Device* CreateDevice(const DeviceConfig& config)
{
    switch (config.mDeviceType)
    {
    case DeviceType::OpenGLES3:
        {
            auto device = new GLESDevice;
            device->Init(config);
            return device;
        }
    default:
        {
            LOG_CRITICAL("Device", "Device {0} is not supported yet", config.mDeviceType);
            return nullptr;
        }
    }
}

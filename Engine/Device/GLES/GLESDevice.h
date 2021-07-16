#pragma once
#include "Device.h"

class GLESDevice : public Device
{
protected:
    void DoInit(const DeviceConfig& config) override;

    void DoDestroy() override;
};

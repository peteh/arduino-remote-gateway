#ifndef CODEID_DEVICE_SWITCH_H
#define CODEID_DEVICE_SWITCH_H
#include <RFSender.h>
#include "DeviceSwitch.h"

class CodeIdDeviceSwitch : public IDeviceSwitch
{
public:
    CodeIdDeviceSwitch(IRFSender* rfSender, long codeId);
    virtual void turnOn();
    virtual void turnOff();

    long getCodeId()
    {
        return m_codeId;
    }

private:
    IRFSender* m_rfSender;
    long m_codeId;
};

#endif
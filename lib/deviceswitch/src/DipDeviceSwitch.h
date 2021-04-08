#ifndef DIP_DEVICE_SWITCH_H
#define DIP_DEVICE_SWITCH_H
#include <RFSender.h>
#include "DeviceSwitch.h"

class DipDeviceSwitch : public IDeviceSwitch
{
public:
    DipDeviceSwitch(IRFSender* rfSender, char *code1, char *code2);
    virtual void turnOn();
    virtual void turnOff();

    char *getCode1()
    {
        return m_code1;
    }

    char *getCode2()
    {
        return m_code2;
    }

private:
    IRFSender* m_rfSender;
    char *m_code1;
    char *m_code2;
};

#endif
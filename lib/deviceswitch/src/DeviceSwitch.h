#ifndef IDEVICE_SWITCH_H
#define IDEVICE_SWITCH_H

class IDeviceSwitch
{
public:
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
};

#endif
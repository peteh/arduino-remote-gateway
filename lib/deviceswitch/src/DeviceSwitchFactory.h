#ifndef DEVICE_SWITCH_FACTORY_H
#define DEVICE_SWITCH_FACTORY_H
#include "DeviceSwitch.h"
#include "DipDeviceSwitch.h"
#include "RFSender.h"
class DeviceSwitchFactory
{
public:
    DeviceSwitchFactory(IRFSender* rfSender);
    IDeviceSwitch *create(const char *topic);

    // TODO: make private again and maybe remove static
public:
    enum switch_type
    {
        dip,
        codeid,
        unknown
    };
    static switch_type getSwitchType(const char *topic);

    static bool extractDipCodeFromTopic(const char *topic, char *code1_, char *code2_);
    static bool extractCodeIdFromTopic(const char *topic, long *codeid_);

private:
    IRFSender* m_rfSender;
};
#endif
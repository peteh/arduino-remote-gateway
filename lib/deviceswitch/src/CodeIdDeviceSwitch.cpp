#include <string.h>
#include <iostream>
#include "CodeIdDeviceSwitch.h"

CodeIdDeviceSwitch::CodeIdDeviceSwitch(IRFSender *rfSender, long codeId)
    : m_rfSender(rfSender),
    m_codeId(codeId)
{
}

void CodeIdDeviceSwitch::turnOn()
{
    m_rfSender->send(m_codeId, 24);
}

void CodeIdDeviceSwitch::turnOff()
{
    m_rfSender->send(m_codeId - 8, 24);
}
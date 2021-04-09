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
    for (unsigned int i = 0; i < m_resends; i++)
    {
        m_rfSender->send(m_codeId, 24);
    }
}

void CodeIdDeviceSwitch::turnOff()
{
    for (unsigned int i = 0; i < m_resends; i++)
    {
        // for these switches the 4th bit decides if we turn on or not
        //  we use the turn on code (bit == 1) as the base, so we need to subtract 8
        m_rfSender->send(m_codeId - 8, 24);
    }
}
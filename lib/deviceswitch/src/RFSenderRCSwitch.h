#ifndef RF_SENDER_RCSWITCH_H
#define RF_SENDER_RCSWITCH_H
#include <RCSwitch.h>
#include <RFSender.h>
class RFSenderRCSwitch : public IRFSender
{
public:
    RFSenderRCSwitch(RCSwitch rcSwitch)
        : rcSwitch(rcSwitch)
    {
    }

    void enableTransmit(int transmitterPin)
    {
        rcSwitch.enableTransmit(transmitterPin);
    }

    void enableReceive(int receiverPin)
    {
        rcSwitch.enableReceive(receiverPin);
    }

    void switchOn(char *code1, char *code2) override
    {
        rcSwitch.switchOn(code1, code2);
    }

    void switchOff(char *code1, char *code2) override
    {
        rcSwitch.switchOff(code1, code2);
    }

    void send(unsigned long code, unsigned int length) override
    {
        rcSwitch.send(code, length);
    }

private:
    RCSwitch rcSwitch;
};
#endif
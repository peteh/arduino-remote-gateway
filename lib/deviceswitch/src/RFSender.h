#ifndef RF_SENDER_H
#define RF_SENDER_H
class IRFSender
{
public:
    virtual void switchOn(char *code1, char *code2) = 0;
    virtual void switchOff(char *code1, char *code2) = 0;
    virtual void send(unsigned long code, unsigned int length) = 0;
};
#endif
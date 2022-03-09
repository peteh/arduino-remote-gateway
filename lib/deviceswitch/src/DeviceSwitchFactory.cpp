#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "DeviceSwitchFactory.h"
#include "DeviceSwitch.h"
#include "DipDeviceSwitch.h"
#include "CodeIdDeviceSwitch.h"
#include "RFSender.h"

DeviceSwitchFactory::DeviceSwitchFactory(IRFSender* rfSender)
    : m_rfSender(rfSender)
{
}

DeviceSwitchFactory::switch_type DeviceSwitchFactory::getSwitchType(const char *topic)
{
    if (strstr(topic, "/dip/") != NULL)
    {
        return dip;
    }
    if (strstr(topic, "/codeid/") != NULL)
    {
        return codeid;
    }
    return unknown;
}

bool DeviceSwitchFactory::extractCodeIdFromTopic(const char *topic, long *codeid_)
{
    // rfdevices / [unique identifier] / dip / [codeid] / switch
    const char searchSlash[2] = "/";
    char *tokenSlash;
    char topicSearch [strlen(topic) + 1];
    strcpy(topicSearch, topic);
    tokenSlash = strtok(topicSearch, searchSlash);
    // skip all tokens till "dip"
    for (int i = 0; i < 2 && tokenSlash != NULL; i++)
    {
        tokenSlash = strtok(NULL, searchSlash);
    }

    if (tokenSlash == NULL || strcmp(tokenSlash, "codeid") != 0)
    {
        std::cerr << "Malformed topic" << std::endl;
        // TODO MALFORMED TOPIC
        return false;
    }

    char *code = strtok(NULL, searchSlash);
    tokenSlash = strtok(NULL, searchSlash);
    if (tokenSlash == NULL || strcmp(tokenSlash, "cmd") != 0)
    {
        // TODO: wrong topic
        std::cerr << "WRONG TOPIC, expected /cmd" << std::endl;
        return false;
    }

    // copy codes
    *codeid_ = atol(code);
    return true;
}

bool DeviceSwitchFactory::extractDipCodeFromTopic(const char *topic, char *code1_, char *code2_)
{
    // rfdevices / [unique identifier] / dip / [code1:code2] / switch
    const char searchSlash[2] = "/";
    const char searchDoublePoint[2] = ":";
    const int codeSegmentLength = 5;
    char *tokenSlash;
    char topicSearch [strlen(topic) + 1];
    strcpy(topicSearch, topic);

    tokenSlash = strtok(topicSearch, searchSlash);
    // skip all tokens till "dip"
    for (int i = 0; i < 2 && tokenSlash != NULL; i++)
    {
        tokenSlash = strtok(NULL, searchSlash);
    }

    if (tokenSlash == NULL || strcmp(tokenSlash, "dip") != 0)
    {
        std::cerr << "Malformed topic" << std::endl;
        // TODO MALFORMED TOPIC
        return false;
    }

    char *code = strtok(NULL, searchSlash);
    tokenSlash = strtok(NULL, searchSlash);
    // TODO: this should be configurable
    if (tokenSlash == NULL || strcmp(tokenSlash, "cmd") != 0)
    {
        // TODO: wrong topicRFSenderMock()
        std::cerr << "WRONG TOPIC, expected /cmd" << std::endl;
        return false;
    }

    char *code1 = strtok(code, searchDoublePoint);
    char *code2 = strtok(NULL, searchDoublePoint);

    if (code1 == NULL || code2 == NULL || strlen(code1) != 5 || strlen(code2) != 5)
    {
        // TODO: MALFORMED ADDRESS
        std::cerr << "Malformed address, expected: 00000:00000" << std::endl;
        return false;
    }

    // copy codes
    strncpy(code1_, code1, codeSegmentLength + 1);
    strncpy(code2_, code2, codeSegmentLength + 1);
    return true;
}

IDeviceSwitch *DeviceSwitchFactory::create(const char *topic)
{
    DeviceSwitchFactory::switch_type type = getSwitchType(topic);
    if (type == unknown)
    {
        return NULL;
    }

    if (type == dip)
    {
        char code1[12];
        char code2[12];

        if (!extractDipCodeFromTopic(topic, code1, code2))
        {
            return NULL;
        }
        return new DipDeviceSwitch(m_rfSender, code1, code2);
    }

    if (type == codeid)
    {
        long codeId = 0;

        if (!extractCodeIdFromTopic(topic, &codeId))
        {
            return NULL;
        }
        return new CodeIdDeviceSwitch(m_rfSender, codeId);
    }

    else
        return NULL;
}
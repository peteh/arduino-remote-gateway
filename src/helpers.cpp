
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"

switch_type getSwitchType(char *topic)
{
    if (strstr(topic, "/dip/") != NULL)
    {
        return dip;
    }
    if (strstr(topic, "/codeid/") != NULL){
        return codeid;
    }
    return unknown;
}

bool extractCodeIdFromTopic(char* topic, long* codeid_){
     // rfdevices / [unique identifier] / dip / [codeid] / switch
    const char searchSlash[2] = "/";
    char *tokenSlash;

    tokenSlash = strtok(topic, searchSlash);
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
    if (tokenSlash == NULL || strcmp(tokenSlash, "switch") != 0)
    {
        // TODO: wrong topic
        std::cerr << "WRONG TOPIC, expected /switch" << std::endl;
        return false;
    }

    // copy codes
    *codeid_ = atol(code);
    return true;
}

bool extractDipCodeFromTopic(char *topic, char *code1_, char *code2_)
{
    // rfdevices / [unique identifier] / dip / [code1:code2] / switch
    const char searchSlash[2] = "/";
    const char searchDoublePoint[2] = ":";
    const int codeSegmentLength = 5;
    char *tokenSlash;

    tokenSlash = strtok(topic, searchSlash);
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
    if (tokenSlash == NULL || strcmp(tokenSlash, "switch") != 0)
    {
        // TODO: wrong topic
        std::cerr << "WRONG TOPIC, expected /switch" << std::endl;
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
    strncpy(code1_, code1, codeSegmentLength);
    strncpy(code2_, code2, codeSegmentLength);
    return true;
}

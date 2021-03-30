#include <iostream>


bool extractCodeFromTopic(char* _topic, char* code1_, char* code2_){
    // rfdevices / [unique identifier] / [code] / switch
    const char searchSlash[2] = "/";
    const char searchDoublePoint[2] = ":";
    const int codeSegmentLength = 5;
    char* tokenSlash;

    tokenSlash = strtok(_topic, searchSlash);
    // skip all tokens till "dip"
    for(int i = 0; i < 2 && tokenSlash != NULL; i++)
    {
        tokenSlash = strtok(NULL, searchSlash);
    }

    if(tokenSlash == NULL || strcmp(tokenSlash, "dip") != 0){
        std::cerr << "Malformed topic" << std::endl;
        // TODO MALFORMED TOPIC
        return false;
    }

    char* code = strtok(NULL, searchSlash);
    tokenSlash = strtok(NULL, searchSlash);
    if(tokenSlash == NULL || strcmp(tokenSlash, "switch") != 0){
        // TODO: wrong topic
        std::cerr << "WRONG TOPIC, expected /switch" << std::endl;
        return false;
    }

    char* code1 = strtok(code, searchDoublePoint);
    char* code2 = strtok(NULL, searchDoublePoint);

    if(code1 == NULL
       || code2 == NULL
       || strlen(code1) != 5
       || strlen(code2) != 5){
        // TODO: MALFORMED ADDRESS
        std::cerr << "Malformed address, expected: 00000:00000" << std::endl;
        return false;
    }

    // copy codes
    strncpy(code1_, code1, codeSegmentLength);
    strncpy(code2_, code2, codeSegmentLength);
    return true;
}

int main() {
    char topic[] = "rfdevices/identifier/dip/10000:11000/switch";
    char code1[6] = "";
    char code2[6] = "";

    if(extractCodeFromTopic(topic, code1, code2)){
        std::cout << code1 << std::endl;
        std::cout << code2 << std::endl;
    }

    return 0;
}

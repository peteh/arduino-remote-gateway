#ifndef HELPERS_H
#define HELPERS_H
enum switch_type { dip, codeid, unknown };

bool extractDipCodeFromTopic(char* _topic, char* code1_, char* code2_);
bool extractCodeIdFromTopic(char* topic, long *codeid);

switch_type getSwitchType(char* topic);

#endif
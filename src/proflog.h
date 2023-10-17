#ifndef PROFLOG_H
#define PROFLOG_H

#include "stdio.h"
#include "proflog_config.h"
#include "proflog_version.h"
#include "proflog_definition.h"

#ifdef __cplusplus
extern "C" {
#endif

void l_init();
void l_initStats(enum RecID id);
void l_timeSinceLast(enum RecID id, enum RecID refTo);
void l(enum RecID id);
void l_param_int(enum RecID id,int paramNo, int value);
void l_param_uint(enum RecID id,int paramNo,unsigned int value);
void l_param_str(enum RecID id,int paramNo, char* value);
char *itoa_vitaut1(char *buf, unsigned int val);

#ifdef __cplusplus
}
#endif

#endif //PROFLOG_H

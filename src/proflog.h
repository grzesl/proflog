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
char *itoa_vitaut1(char *buf, unsigned int val);
char *itoa_vitaut1_pl(char *buf, unsigned int val);

#ifdef __cplusplus
}
#endif

#endif //PROFLOG_H

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
void l(enum RecID id);
void l_timeSinceLast(unsigned int id, unsigned int refTo);

#ifdef __cplusplus
}
#endif

#endif //PROFLOG_H

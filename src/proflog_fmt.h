#ifndef PROFLOG_FMT_H
#define PROFLOG_FMT_H

#include "proflog_config.h"

typedef struct _FmtLine {
    unsigned int len;
    unsigned int pos;
    char *data;
}FmtLine;

void fmt_init(FmtLine *line, char*buff, int len);
int fmt_append_str(FmtLine *line, const char* c);
int fmt_append_char(FmtLine *line, int c);
int fmt_append_uint(FmtLine *line, unsigned int val, int align, int fillchar);
int fmt_append_int(FmtLine *line, int val, int align, int fillchar);
int fmt_append_mem(FmtLine *line, char* c, int len);
int fmt_append_datetime(FmtLine *line, int year, int mon, int day, int hour, int min, int sec, int msec);
char *itoa_vitaut1(char *buf, unsigned int val);
#endif //PROFLOG_FMT_H
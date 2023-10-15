#ifndef PROFLOG_FMT_H
#define PROFLOG_FMT_H

#include "proflog_config.h"

typedef struct _FmtLine {
    unsigned long len;
    unsigned long pos;
    char data [MAX_LOG_LINE_LEN + 1];
}FmtLine;

void fmt_init(FmtLine *line);
int fmt_append_str(FmtLine *line, const char* c);
int fmt_append_char(FmtLine *line, int c);
int fmt_append_uint(FmtLine *line, unsigned int val, int align, int fillchar);
int fmt_append_int(FmtLine *line, int val, int align, int fillchar);
int fmt_append_mem(FmtLine *line, char* c, int len);

#endif //PROFLOG_FMT_H
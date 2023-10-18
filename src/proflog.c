
#define PREFLOG_DEFINITION
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "proflog_definition.h"
#include "proflog_fmt.h"
#include "proflog_tofile.h"


unsigned int l_getTickCount(){
    return clock();
}

void l_initStats(enum RecID id) {

    if (LOG_LEVEL == 5)
        return;

    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    unsigned long currentTick = l_getTickCount();
    item->counter = 0;
    item->firstPrintTime = currentTick;
    item->lastPrintTime = currentTick;
}

void l_init() {
    if(LOG_LEVEL == 5)
        return;

    if(*logDef.configInfo.logFilename){
        tofile_init(logDef.configInfo.logFilename);
    }

    fmt_init(&logDef.line, logDef.data, MAX_LOG_LINE_LEN);
    


    fmt_append_str(&logDef.line, "--- ");
    fmt_append_str(&logDef.line, logDef.records[L_PROF_LOG_INFO].catalog.profLogInfo.logName);
    fmt_append_char(&logDef.line, ' ');
    fmt_append_str(&logDef.line, logDef.records[L_PROF_LOG_INFO].catalog.profLogInfo.version);
    fmt_append_str(&logDef.line, " -- ");
    fmt_append_str(&logDef.line, logDef.records[L_PROF_LOG_INFO].catalog.appLogInfo.appName);
    fmt_append_char(&logDef.line, ' ');
    fmt_append_str(&logDef.line, logDef.records[L_PROF_LOG_INFO].catalog.appLogInfo.version);
    fmt_append_str(&logDef.line, "---\n");

    fmt_append_str(&logDef.line, "--- Configuration: ");
    if(*logDef.configInfo.comMedium){
        fmt_append_str(&logDef.line, " Comm medium: ");
        fmt_append_str(&logDef.line, logDef.configInfo.comMedium);
    }

    if (*logDef.configInfo.logFilename)
    {
        fmt_append_str(&logDef.line, " Log file name: ");
        fmt_append_str(&logDef.line, logDef.configInfo.logFilename);
    }

    fmt_append_str(&logDef.line, " Log level: ");
    fmt_append_str(&logDef.line, LogLevelString[logDef.configInfo.logLevel]);
    fmt_append_str(&logDef.line, " Log to stdout: ");
    fmt_append_int(&logDef.line, logDef.configInfo.options.bit.logToStdout, 0, 0);


    fmt_append_char(&logDef.line, '\n');
    tofile_append(logDef.line.data, logDef.line.pos);

    logDef.line.pos -=1;
    fmt_append_char(&logDef.line, '\0');
    puts(logDef.line.data);

    l_initStats(L_FINISH_MSG);
}

void l_print_param(FmtLine *line, ParamLogDef *def) {

    if (LOG_LEVEL == 5)
        return;

        fmt_append_str(line, def->name);
        switch(def->type)
        {
            case PARAM_INT:
            {
                fmt_append_int(line, def->value_int, 0,0);
                break;
            }
            case PARAM_UINT:
            {
                fmt_append_int(line, def->value_uint,0,0);
                break;
            }            
            case PARAM_STRING:
            {
                fmt_append_str(line, def->value_str);
            }
            break;
        }
        fmt_append_char(&logDef.line, ' ');

}



void l(enum RecID id) {

    if(LOG_LEVEL == 5)
        return;

    LogItemDef *item = &logDef.records[id].catalog.logItemDef;

    if(item->level < LOG_LEVEL - 1)
        return;

    fmt_init(&logDef.line, logDef.data, MAX_LOG_LINE_LEN);
    logDef.count++;


    unsigned long currentTick = l_getTickCount();

    if(logDef.configInfo.options.bit.logToStdout) {

        fmt_append_str(&logDef.line,LogLevelString[item->level]);
        fmt_append_str(&logDef.line, " (");
        fmt_append_uint(&logDef.line, logDef.count, 6, '_');

        fmt_append_str(&logDef.line, ") ");
        fmt_append_str(&logDef.line, item->name);
        if(*item->name)
            fmt_append_char(&logDef.line, ' ');

        for(int i=0;i<MAX_LOG_PARAM_COUNT;i++)
        {
            if(item->params[i].type == PARAM_NONE)
                break;
            l_print_param(&logDef.line, &item->params[i]);
        }

        if (item->options.bit.printCounter)
        {
            fmt_append_char(&logDef.line, '[');
            fmt_append_uint(&logDef.line, item->counter, 6, '_');
            fmt_append_str(&logDef.line, "] ");
        }
        
        if(item->timeSinceLastRefTo){
            fmt_append_uint(&logDef.line, currentTick - item->timeSinceLastRefTo, 8, '_');
            fmt_append_str(&logDef.line, "[ms] ");
        }

        if(item->options.bit.printTotalTime){
            fmt_append_uint(&logDef.line, currentTick - item->firstPrintTime, 8, '_');
            fmt_append_str(&logDef.line, "[ms] ");
        }
        if(item->options.bit.printTimeSinceLast){
            fmt_append_uint(&logDef.line, currentTick - item->lastPrintTime, 8, '_');
            fmt_append_str(&logDef.line, "[ms] ");
        }

        if (item->options.bit.noAppendCR == 0)
        {
            fmt_append_char(&logDef.line, '\n');
            tofile_append(logDef.line.data, logDef.line.pos);
            logDef.line.pos-=1;
        }

       
        fmt_append_char(&logDef.line, '\0');

        if (item->options.bit.noAppendCR == 0)
            puts(logDef.line.data);
        else printf("%s", logDef.line.data);
    }

    //update stats:
    item->lastPrintTime = currentTick;
    if(item->firstPrintTime == 0)
        item->firstPrintTime = currentTick;
    item->counter ++;
}


void l_param_str(enum RecID id, int param, char *str) {
    if (LOG_LEVEL == 5)
        return;

    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    strncpy(item->params[param].value_str, str, MAX_LOG_PARAM_STRING_LEN);
    item->params[param].type = PARAM_STRING;
}

void l_param_uint(enum RecID id,int param, unsigned int value) {

    if (LOG_LEVEL == 5)
        return;

    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    item->params[param].value_uint = value;
    item->params[param].type = PARAM_UINT;
}

void l_param_int(enum RecID id,int param, int value) {

    if (LOG_LEVEL == 5)
        return;

    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    item->params[param].value_int = value;
    item->params[param].type = PARAM_INT; 
}

void l_strint(char*str, int val1){
    if (LOG_LEVEL == 5)
        return;

    l_param_str(L_DEFAULT, 0, str);
    l_param_int(L_DEFAULT, 1, val1);
    l(L_DEFAULT);
}


void l_old(enum RecID id) {

    if(LOG_LEVEL == 5)
        return;

    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
        
    if(item->level < LOG_LEVEL - 1)
        return;

    logDef.count++;

    unsigned long currentTick = l_getTickCount();

    if(logDef.configInfo.options.bit.logToStdout) {

        printf("%8s ", LogLevelString[item->level]);

        if (item->options.bit.printSeqNumber)
            printf("(%06d) ", logDef.count);
        else 
            printf("%8s ", "");

        printf("%-16s ", item->name);

        if (item->options.bit.printCounter)
            printf("[%06d] ", item->counter);

        if(item->timeSinceLastRefTo)
            printf("%8ld {<->} [ms] ", currentTick - item->timeSinceLastRefTo);

        if(item->options.bit.printTotalTime)
            printf("%8ld (<->) [ms] ", currentTick - item->firstPrintTime);

        if(item->options.bit.printTimeSinceLast)
            printf("%8ld |<->| [ms] ", currentTick - item->lastPrintTime);


        if (item->options.bit.noAppendCR == 0)
            printf("\n");
    }

    //update stats:
    item->lastPrintTime = currentTick;
    if(item->firstPrintTime == 0)
        item->firstPrintTime = currentTick;
    item->counter ++;
}

unsigned int l_timeSinceLast(enum RecID id, enum RecID refTo) {
    if (LOG_LEVEL == 5)
        return 0;

    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    LogItemDef *refitem = &logDef.records[refTo].catalog.logItemDef;

    item->timeSinceLastRefTo = refitem->lastPrintTime;
    return item->timeSinceLastRefTo;
}

void l_finish() {
    if (LOG_LEVEL == 5)
        return;

    l(L_FINISH_MSG);
    tofile_close();
}
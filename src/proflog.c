
#define PREFLOG_DEFINITION
#include "stdio.h"
#include "time.h"
#include "proflog_definition.h"
#include "proflog_fmt.h"

void l_init() {
    fmt_init(&logDef.line);
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
    fmt_append_char(&logDef.line, '\0');
    puts(logDef.line.data);
}

void l_init_old() {
    printf("--- %s %s -- %s %s --- \n",
    logDef.records[L_PROF_LOG_INFO].catalog.profLogInfo.logName,
    logDef.records[L_PROF_LOG_INFO].catalog.profLogInfo.version,
    logDef.records[L_LOG_APP_INFO].catalog.appLogInfo.appName,
    logDef.records[L_LOG_APP_INFO].catalog.appLogInfo.version);

    printf("%s", "--- Configuration: ");
    if(*logDef.configInfo.comMedium)
        printf("Comm medium: %s " , logDef.configInfo.comMedium);
    if(*logDef.configInfo.logFilename)
        printf("Log file name: %s ", logDef.configInfo.logFilename);
    printf("Log level: %s ", LogLevelString[logDef.configInfo.logLevel]);
    printf("Log to stdout: %d ", logDef.configInfo.options.bit.logToStdout);
    printf("\n");
}

unsigned int l_getTickCount(){
    return clock();
}

void l_initStats(enum RecID id) {
    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    unsigned long currentTick = l_getTickCount();
    item->counter = 0;
    item->firstPrintTime = currentTick;
    item->lastPrintTime = currentTick;
}

void l(enum RecID id) {
    fmt_init(&logDef.line);
    logDef.count++;
    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    unsigned long currentTick = l_getTickCount();

    if(logDef.configInfo.options.bit.logToStdout) {

        fmt_append_str(&logDef.line,LogLevelString[item->level]);
        fmt_append_str(&logDef.line, " (");
        fmt_append_uint(&logDef.line, logDef.count, 6, '_');

        fmt_append_str(&logDef.line, ") ");
        fmt_append_str(&logDef.line, item->name);
        fmt_append_char(&logDef.line, ' ');

        if (item->options.bit.printCounter)
        {
            fmt_append_char(&logDef.line, '[');
            fmt_append_uint(&logDef.line, item->counter, 6, '_');
            fmt_append_str(&logDef.line, "] ");
        }
        
        if(item->timeSinceLastRefTo){
            fmt_append_uint(&logDef.line, currentTick - item->timeSinceLastRefTo, 8, '_');
            fmt_append_str(&logDef.line, " {<->} [ms] ");
        }

        if(item->options.bit.printTotalTime){
            fmt_append_uint(&logDef.line, currentTick - item->firstPrintTime, 8, '_');
            fmt_append_str(&logDef.line, " (<->) [ms] ");
        }
        if(item->options.bit.printTimeSinceLast){
            fmt_append_uint(&logDef.line, currentTick - item->lastPrintTime, 8, '_');
            fmt_append_str(&logDef.line, " |<->| [ms] ");
        }
        fmt_append_char(&logDef.line, '\0');
        if (item->options.bit.noAppendCR == 0)
            puts(&logDef.line.data);
        else printf("%s", logDef.line.data);
    }

    //update stats:
    item->lastPrintTime = currentTick;
    if(item->firstPrintTime == 0)
        item->firstPrintTime = currentTick;
    item->counter ++;
}

void l_old(enum RecID id) {

    logDef.count++;
    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
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

void l_timeSinceLast(enum RecID id, enum RecID refTo) {
    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    LogItemDef *refitem = &logDef.records[refTo].catalog.logItemDef;

    item->timeSinceLastRefTo = refitem->lastPrintTime;
    l(id);
}

#define PREFLOG_DEFINITION
#include "stdio.h"
#include "time.h"
#include "proflog_definition.h"

void l_init() {
    printf("--- %s %s -- %s %s --- \n",
    logDef.records[L_PROF_LOG_INFO].catalog.profLogInfo.logName,
    logDef.records[L_PROF_LOG_INFO].catalog.profLogInfo.version,
    logDef.records[L_LOG_APP_INFO].catalog.appLogInfo.appName,
    logDef.records[L_LOG_APP_INFO].catalog.appLogInfo.version);

    printf("%s", "Configuration: ");
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

void l_initStats(unsigned int id) {
    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    unsigned long currentTick = l_getTickCount();
    item->counter = 0;
    item->firstPrintTime = currentTick;
    item->lastPrintTime = currentTick;
}



void l(unsigned int id) {

    logDef.count++;
    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    unsigned long currentTick = l_getTickCount();

    if(logDef.configInfo.options.bit.logToStdout) {

        printf("%8s ", LogLevelString[item->level]);

        if (item->options.bit.printSeqNumber)
            printf("(%06d) ", logDef.count);

        printf("%s ", item->name);

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

void l_timeSinceLast(unsigned int id, unsigned int refTo) {
    LogItemDef *item = &logDef.records[id].catalog.logItemDef;
    LogItemDef *refitem = &logDef.records[refTo].catalog.logItemDef;

    item->timeSinceLastRefTo = refitem->lastPrintTime;
    l(id);
}
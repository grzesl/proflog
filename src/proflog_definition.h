#ifndef PROFLOG_DEFINITION_H
#define PROFLOG_DEFINITION_H

#include "proflog_version.h"
#include "proflog_types.h"

#define EACH_LOG_RECORD_ID(_) \
        _(L_PROF_LOG_INFO)   \
        _(L_LOG_APP_INFO)  \
        _(L_SYSTEM_START)   \
        _(L_SYSTEM_STOP)   \
        _(L_MAIN_LOOP_BEGIN) \
        _(L_TASK) \
        _(L_LONG_OPERATION) \
        _(L_CONVERSION) \
        _(L_ERROR) \
        _(L_WARNING) \
        _(L_INFO) \
        _(L_CRITICAL) \
        _(L_MUSTBE) \
        _(L_FINISH_MSG) \

        

enum RecID {
    EACH_LOG_RECORD_ID(GENERATE_ENUM)
};

//#define PREFLOG_DEFINITION
#ifdef PREFLOG_DEFINITION

LogDef logDef = {
    sizeof(logDef.records) / sizeof(LogItemDef),
    {"proflog.zip", "", INFO, .options.bit.logToStdout = 1},
    {
        {L_PROF_LOG_INFO, PROF_LOG_INFO,{.profLogInfo = {PROF_LOG_NAME, PROF_LOG_VERSION}}},
        {L_LOG_APP_INFO, LOG_APP_INFO, {.appLogInfo = {"MyExampleApp", "1.00"}}},
        {L_SYSTEM_START, LOG_ITEM_DEF, {.logItemDef = {MUSTBE, "System starting..."}}},
        {L_SYSTEM_STOP, LOG_ITEM_DEF, {.logItemDef = {MUSTBE, "System stopping...", .options.bit.printTotalTime = 1}}},
        {L_MAIN_LOOP_BEGIN, LOG_ITEM_DEF, {.logItemDef = {INFO, "Main loop", .options.bit.printTimeSinceLast = 1, .options.bit.printSeqNumber = 1, .options.bit.printCounter = 1}}},
        {L_TASK, LOG_ITEM_DEF, {.logItemDef = {INFO, "Task", .options.bit.printTimeSinceLast = 1, .options.bit.printSeqNumber = 1, .options.bit.printCounter = 1}}},
        {L_LONG_OPERATION, LOG_ITEM_DEF, {.logItemDef = {WARNING, "Long operation", .options.bit.printCounter = 1}}},
        {L_CONVERSION, LOG_ITEM_DEF, {.logItemDef = {INFO, "Conversion", {{"from:"},{"to:"}} , .options.bit.printTimeSinceLast = 1}}},
        {L_ERROR ,LOG_ITEM_DEF, {.logItemDef = {ERROR, "", {{""},{""}}}}},
        {L_WARNING ,LOG_ITEM_DEF, {.logItemDef = {WARNING, "", {{""},{""}}}}},
        {L_INFO ,LOG_ITEM_DEF, {.logItemDef = {INFO, "", {{""},{""}}}}},
        {L_CRITICAL ,LOG_ITEM_DEF, {.logItemDef = {CRITICAL, "", {{""},{""}}}}},
        {L_MUSTBE ,LOG_ITEM_DEF, {.logItemDef = {MUSTBE, "", {{""},{""}}}}},
        {L_FINISH_MSG, LOG_ITEM_DEF, {.logItemDef = {MUSTBE, "Finishing loging...", .options.bit.printTotalTime = 1}}},
    }};

#endif //PREFLOG_DEFINITION


#endif //PROFLOG_DEFINITION_H
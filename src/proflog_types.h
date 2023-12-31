#ifndef PROFLOG_TYPES_H
#define PROFLOG_TYPES_H

#include "proflog_fmt.h"

#define EACH_LOG_LEVEL(_) \
        _(INFO)   \
        _(WARNING)  \
        _(ERROR)   \
        _(CRITICAL)  \
        _(MUSTBE)  \

#define EACH_LOG_TYPE(_) \
        _(PROF_LOG_INFO)   \
        _(LOG_APP_INFO)  \
        _(PROF_LOG_CFG)   \
        _(LOG_ITEM_DEF)   \

#define EACH_PARAM_TYPE(_) \
        _(PARAM_NONE)   \
        _(PARAM_INT)   \
        _(PARAM_UINT)  \
        _(PARAM_STRING)   \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum LogLevel {
    EACH_LOG_LEVEL(GENERATE_ENUM)
};

static const char *LogLevelString[] = {
    EACH_LOG_LEVEL(GENERATE_STRING)
};

enum LogType {
    EACH_LOG_TYPE(GENERATE_ENUM)
};

static const char *LogTypeString[] = {
    EACH_LOG_TYPE(GENERATE_STRING)
};

enum ParamType  {  
    EACH_PARAM_TYPE(GENERATE_ENUM)
};

typedef struct _LogHdr {
    int  id;
    enum  LogType header;
}LogHdr;

typedef struct _ProfLogInfo {
    char logName  [10];
    char version [10];
}ProfLogInfo;

typedef struct _ParamLogDef {
    char name[MAX_LOG_PARAM_NAME_LEN + 1];
    enum ParamType type;
    int value_int;
    unsigned int value_uint;
    char value_str[MAX_LOG_PARAM_STRING_LEN + 1];
} ParamLogDef;

typedef struct _LogItemDef {
    enum LogLevel level;
    char name [64];
    ParamLogDef params[MAX_LOG_PARAM_COUNT];
    union
    {
        struct
        {
            unsigned int noAppendCR : 1;
            unsigned int printTotalTime : 2;
            unsigned int printTimeSinceLast : 3;
            unsigned int printCounter: 4;
            unsigned int printSeqNumber: 5;
            unsigned int unused : 27;
        } bit;
        unsigned int value;
    } options;

    //statistics - should be last
    unsigned int firstPrintTime;
    unsigned int lastPrintTime;
    unsigned int timeSinceLastRefTo;
    unsigned int counter;
} LogItemDef;

typedef struct _LogAppInfo {
    char appName [32];
    char version [32];    
} LogAppInfo;

typedef struct _LogCfg {
    char logFilename [256];
    char comMedium [64];   
    unsigned char logLevel;
    union {
        struct {
            unsigned int logToStdout: 1;
            unsigned int unused: 31;
        } bit;
        unsigned int value;
    } options;
} LogCfg;

typedef union _LogCatalog
{
    ProfLogInfo profLogInfo;
    LogAppInfo  appLogInfo;
    LogItemDef  logItemDef;
}LogCatalog;

typedef struct _LogRecordDef {
    LogHdr header;
    LogCatalog catalog;
} LogRecordDef;

typedef struct _LogDef {
    int recCount;
    LogCfg configInfo;
    LogRecordDef records[MAX_LOG_DEFINITIONS_COUNT];
    FmtLine line;
    char data [MAX_LOG_LINE_LEN + 1];
    //statistics should be at end
    unsigned long count;
}LogDef;


#endif //PROFLOG_TYPES_H
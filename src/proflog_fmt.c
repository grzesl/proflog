
#include "string.h"
#include "proflog_config.h"
#include "proflog_fmt.h"

#if LOG_LEVEL != 5

void fmt_init(FmtLine *line, char*buff, int len) {
    line->len = len;
    line->pos = 0;
    line->data = buff;
}

int fmt_append_char(FmtLine *line, int c){
    if(line->len == line->pos)
        return 0;

    line->data[line->pos] = c;
    line->pos++;
    return 1;
}

int fmt_append_mem(FmtLine *line, char* c, int len){
    char* p = c;
    while(len--){
        if(!fmt_append_char(line, *p++))
            break;
    }
    return (int)(p - c - 1);
}

int fmt_append_str(FmtLine *line, const char* c){
    char* p = (char*)c;
    if(!*p) return 0;
    while(*p){
        if(!fmt_append_char(line, *p++))
            break;
    }
    return (int)(p - c - 1);
}

static unsigned short const str100p[100] = {
    0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830, 0x3930,
    0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731, 0x3831, 0x3931,
    0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632, 0x3732, 0x3832, 0x3932,
    0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533, 0x3633, 0x3733, 0x3833, 0x3933,
    0x3034, 0x3134, 0x3234, 0x3334, 0x3434, 0x3534, 0x3634, 0x3734, 0x3834, 0x3934,
    0x3035, 0x3135, 0x3235, 0x3335, 0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935,
    0x3036, 0x3136, 0x3236, 0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936,
    0x3037, 0x3137, 0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737, 0x3837, 0x3937,
    0x3038, 0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938,
    0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839, 0x3939, };

char *itoa_vitaut1(char *buf, unsigned int val)
{
    char *p = &buf[10];

    *p = '\0';

    while(val >= 100)
    {
        unsigned int const old = val;

        p -= 2;
        val /= 100;
        memcpy(p, &str100p[old - (val * 100)], sizeof(unsigned short));
    }

    p -= 2;
    memcpy(p, &str100p[val], sizeof(unsigned short));

    return &p[val < 10];
}

int fmt_append_align(FmtLine *line, int align, int fillchar) {
    int len = align;
    while(len > 0)
    {
        if(fmt_append_char(line, fillchar) == 0)
            break;
        len--;
    }
    return align - len;
}

//itoa_vitaut1
int fmt_append_uint(FmtLine *line, unsigned int val, int align, int fillchar){
    int digit = 0;
    int i = 1;
    char buff[10];
    char *resstr;

    resstr = itoa_vitaut1(buff, val);

    int len = 10 - (int) (resstr - buff);

    align = align-len;

    if(align < 0)
        align = 0;

    int alignlen = fmt_append_align(line,align, fillchar); 
       
    return fmt_append_mem(line, resstr, len) + alignlen;
}

int fmt_append_int(FmtLine *line, int val, int align, int fillchar){
    char buff[10];
    char *resstr;
    int addminus = 0;
    if(val < 0){
        addminus = 1;
        val = val * -1;
    }

    resstr = itoa_vitaut1(buff, val);

    int len = 10 - (int) (resstr - buff);

    align = align-len-addminus;

    if(align < 0)
        align = 0;

    int alignlen = fmt_append_align(line,align, fillchar); 

    if(addminus)
    {
        fmt_append_char(line, '-');
    }

    return fmt_append_mem(line, resstr, len) + alignlen + addminus;
}


int fmt_append_datetime(FmtLine *line, int year, int mon, int day, int hour, int min, int sec, int msec){
    fmt_append_uint(line, year, 4, '0');
    fmt_append_char(line, '-');
    fmt_append_uint(line, mon, 2, '0');
    fmt_append_char(line, '-');
    fmt_append_uint(line, day, 2, '0');
    fmt_append_char(line, ' ');
    fmt_append_uint(line, hour, 2, '0');
    fmt_append_char(line, ':');
    fmt_append_uint(line, min, 2, '0');
    fmt_append_char(line, ':');
    fmt_append_uint(line, sec, 2, '0');
    fmt_append_char(line, '.');
    fmt_append_uint(line, msec, 3, '0');

}

#endif //#if LOG_LEVEL != 5
#include <stdlib.h>
#include "proflog.h"
#include "proflog_fmt.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif // _WIN32

void sleep(int milliseconds)
{
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif // _WIN32
}

int main ()
{

    l_init();
    l_initStats(L_SYSTEM_STOP); //time counters init
    for (int i = 0;  i < 1; i ++ ) {
        l(L_MAIN_LOOP_BEGIN);

        char buff[32];
        l_initStats(L_CONVERSION); //time counters init
        for (int j = 0; j < 100; j++)
        {
            l(L_TASK);
        }
        l_info("New logs are fast, count: ", 100);
        l_param_str(L_CONVERSION,0,"logs");
        l_param_str(L_CONVERSION,1,"amazing");
        l(L_CONVERSION);
     
        for (int j = 0; j < 100; j++)
        {
            l_old(L_TASK);
        }

        l_info("Slow logs, loop count:", 100);
        l_param_str(L_CONVERSION,0,"logs");
        l_param_str(L_CONVERSION,1,"default");
        l(L_CONVERSION);

        unsigned int timeSince = l_timeSinceLast(L_CONVERSION, L_MAIN_LOOP_BEGIN);
        if(timeSince > 400)
            l_error("Too slow code, about:",timeSince);
    }


    l(L_SYSTEM_STOP);

    l_finish();

    return 0;
}
#include <stdlib.h>
#include "proflog.h"

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
    l(L_SYSTEM_START);

    for (int i = 0;  i < 10; i ++ ) {
        l(L_MAIN_LOOP_BEGIN);

        int msec = rand()%1000;
        sleep(msec);

        if(msec > 500)
        {
            l_timeSinceLast(L_LONG_OPERATION, L_MAIN_LOOP_BEGIN);
        }
    }


    l(L_SYSTEM_STOP);

    return 0;
}
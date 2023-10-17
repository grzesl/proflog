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

char *itoa_vitaut1(char *buf, unsigned int val);
char *itoa_vitaut1_glpro(char *buf, unsigned int val);

int main ()
{

    l_init();

    for (int i = 0;  i < 1; i ++ ) {
        l(L_MAIN_LOOP_BEGIN);

        char buff[32];
        l_initStats(L_CONVERSION);
        for (int j = 0; j < 100; j++)
        {
            //itoa(j, buff, 10);
            l(L_MAIN_LOOP_BEGIN);
        }
        l_param_str(L_CONVERSION,0,"logs");
        l_param_str(L_CONVERSION,1,"amazing");
        l(L_CONVERSION);
     
        for (int j = 0; j < 100; j++)
        {
            //itoa_vitaut1(buff, j);
            l_old(L_MAIN_LOOP_BEGIN);
        }
        l_param_str(L_CONVERSION,0,"logs");
        l_param_str(L_CONVERSION,1,"default");
        l(L_CONVERSION);

        /* int msec = rand()%1000;
         sleep(msec);

         if(msec > 500)
         {
             l_timeSinceLast(L_LONG_OPERATION, L_MAIN_LOOP_BEGIN);
         }*/
    }


    l(L_SYSTEM_STOP);

    return 0;
}
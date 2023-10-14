# proflog
Ultra smart and structurized logging library for C/C++


```
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
```

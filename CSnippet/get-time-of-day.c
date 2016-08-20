#include <stdio.h>
#include <sys/time.h>
//#include <time.h>

int main () {

    char buf[1024];
    long sec;
    struct timeval tv;
    struct tm *tm;
    gettimeofday(&tv, NULL);
    sec = tv.tv_sec;
    tm = localtime(&sec);
    sprintf(buf, "%04d-%02d-%2d %02d:%02d:%03d",
            tm->tm_year+1990,
            tm->tm_mon+1,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec);
    printf("%s\n", buf);
}

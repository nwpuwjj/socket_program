#include "unp.h"

static void connect_alarm(int);

int connect_timeo(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
    Sigfunc *sigfunc;
    int n;
    sigfunc = Signal(SIGALRM,connect_alarm);
    if(alarm(nsec)!=0)
    {
        printf("[connect_timeo]: alarm was already set\n");
        exit(-1);
    }

    if( (n=connect(sockfd,saptr,salen))<0)
    {
        close(sockfd);
        if(errno==EINTR)
            errno=ETIMEDOUT;
    }
    alarm(0);   /*turn off the alarm*/
    signal(SIGALRM,sigfunc); /*restore previous signal handler*/

    return n;
}

static void connect_alarm(int signo)
{
    return; /*just interrupt the connect()*/
}
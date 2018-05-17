//对pselect 给出一个尽管不正确然后简单的实现

#include "unp.h"

int pselect(int nfds, fd_set *rset, fd_set *wset, fd_set *xset, const struct timespec *ts,const sigset_t *sigmask)
{
    int n;
    struct timeval tv;
    sigset_t savemask;

    if(ts!=NULL)
    {
        tv.tv_sec= ts->tv_sec;
        tv.tv_usec= ts->tv_nsec/1000; /*nanosec->microsec*/
    }

    sigprocmask(SIG_SETMASK,sigmask,&savemask); /*caller's mask*/
    n=select(nfds, rset, wset, xset, (ts==NULL)?NULL:&tv);
    sigprocmask(SIG_SETMASK, &savemask, NULL); /*restore mask */

    return n;
}

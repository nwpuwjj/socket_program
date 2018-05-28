#include "ping.h"
void sig_arm(int signo)
{
    (*pr->fsend)();
    alarm(1);
    return;
}

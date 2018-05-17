#include "unp.h"

void recv_all(int recvfd,socklen_t salen)
{
    int n;
    char line[MAXLINE+1];
    socklen_t len;
    struct sockaddr *safrom;

    safrom=malloc(salen);
    for(;;)
    {
        len=salen;
        n=recvfrom(recvfd, line, MAXLINE,0,safrom,&len);
        line[n]=0;
        printf("from %s: %s",sock_ntop(safrom,len),line);
    }
}

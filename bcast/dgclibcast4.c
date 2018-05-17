#include "unp.h"

static void recvfrom_alarm(int);

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int n;
    const int on=1;
    char sendline[MAXLINE],recvline[MAXLINE+1];
    fd_set rset;
    sigset_t sigset_alrm, sigset_empty;
    socklen_t len;
    struct sockaddr *preply_addr;

    preply_addr=malloc(servlen);

    setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST, &on, sizeof(on));

    FD_ZERO(&rset);

    sigemptryset(&sigset_empty);
    sigemptryset(&sigset_alrm);
    sigaddset(&sigset_alrm,SIGALRM);

    signal(SIGALRM, recvfrom_alarm);

    while(fgets(sendline,MAXLINE,fp)!=NULL)
    {
        sendto(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);

        sigprocmask(SIG_BLOCK,&sigset_alrm,NULL);
        alarm(5);
        for(;;)
        {
            FD_SET(sockfd,&rset);
            n=pselect(sockfd+1,&rset,NULL,NULL,NULL,&sigset_empty);
            if(n<0)
            {
                if(errno==EINTR)
                    break;
                else
                    err_sys("pselect error");
            }
            else if (n!=1)
                err_sys("pselect error: return %d",n);

            len=servlen;
            n=recvfrom(sockfd,recvline,MAXLINE,0,preply_addr,&len);
            recvline[n]=0; /*null terminate */
            printf("from %s: %s",sock_ntop_host(preply_addr,len), recvline);
        }
    }

    free(preply_addr);
}

static void recvfrom_alarm(int signo)
{
    return;
}

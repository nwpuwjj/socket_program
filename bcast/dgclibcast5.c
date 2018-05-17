#include "unp.h"
#include <setjmp.h>

static void recvfrom_alarm(int);
static sigjmp_buf jmpbuf;

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int n;
    const int on =1;
    char sendline[MAXLINE], recvline[MAXLINE+1];
    socklen_t len;
    struct sockaddr *preply_addr;

    preply_addr=malloc(servlen);
    setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));
    signal(SIGALARM,recvfrom_alarm);

    while(fgets(sendline,MAXLINE,fp)!=NULL)
    {
        sendto(sockfd,sendline,strlen(sendline),0,pservaddr, servlen);

        alarm(5);
        for(;;)
        {
            if(sigsetjmp(jmpbuf,1)!=0)
                break;

            len=servlen;
            n=recvfrom(sockfd,recvline,MAXLINE,0,preply_addr,&len);
            recvline[n]=0;//null terminate
            printf("from %s: %s", sock_ntop_host(preply_addr,len),recvline);
        }
    }

    free(preply_addr);
}

static void recvfrom_alarm(int signo)
{
    siglongjmp(jmpbuf,1);
}
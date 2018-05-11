#include "unp.h"

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int n;
    char sendline[MAXLINE], recvline[MAXLINE+1];
    connect(sockfd, (SA *)pservaddr, servlen);

    while(fgets(sendline,MAXLINE,fp)!=NULL)
    {
        write(sockfd,sendline,strlen(sendline));
        n=read(sockfd,recvline,MAXLINE);
        recvline[n]=0; /*null terminate*/
        fputs(recvline,stdout);
    }
}

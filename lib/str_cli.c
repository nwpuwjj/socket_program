#include "unp.h"

void str_cli(FILE *fp,int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while(fgets(sendline,MAXLINE,fp)!=NULL)
    {
        written(sockfd,sendline,strlen(sendline));

        if((readline(sockfd,recvline,MAXLINE)==0))
        {
            printf("strcli: server terminated prematurely\n");
            exit(-1);
        }

        fputs(recvline,stdout);
    }
}


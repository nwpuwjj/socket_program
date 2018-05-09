#include "unp.h"

void str_cli(FILE *fp, int sockfd)
{
    int maxfdp1;
    fd_set rset;
    char sendline[MAXLINE],recvline[MAXLINE];

    FD_ZERO(&rset);
    for(;;)
    {
        FD_SET(fileno(fp),&rset);
        FD_SET(sockfd,&rset);

        maxfdp1=max(fileno(fp),sockfd)+1;
        select(maxfdp1,&rset,NULL,NULL,NULL);

        if(FD_ISSET(sockfd,&rset))
        {
            /* socket is readable */
            if(readline(sockfd,recvline,MAXLINE)==0)
            {
                printf("[str_cli]:server terminated prematurely\n");
                exit(-1);
            }
            fputs(recvline,stdout);
        }

        if(FD_ISSET(fileno(fp),&rset))
        {
            /*input is readable*/
            if(fgets(sendline,MAXLINE,fp)==NULL)
                return; /*all done*/
            writen(sockfd,sendline,strlen(sendline));
        }
    }
}

#include "unp.h"

void str_cli(FILE *fp, int sockfd)
{
    int maxfdp1,stdineof;
    fd_set rset;
    char buf[MAXLINE];
    int n;

    stdineof=0;
    FD_ZERO(&rset);

    for(;;)
    {
        if(stdineof == 0)
            FD_SET(fileno(fp),&rset);

        FD_SET(sockfd,&rst);
        maxfdp1=max(fileno(fp),sockfd)+1;
        select(maxfdp1,&rset,NULL,NULL,NULL);

        if(FD_ISSET(sockfd,&rset))
        {
            //socket is readable
            if((n=read(sockfd,buf,MAXLINE))==0)
            {
                if(stdineof==1)
                    return; /*normal termination */
                else
                {
                    printf("[str_cli]:server teminated prematurely\n");
                    exit(-1);
                }
                write(fileno(stdout),buf,n);
            }
        }

        if(FD_ISSET(fileno(fp),&rset))
        {
            /*input is readable*/
            if((n=read(fileno(fp),buf,MAXLINE))==0)
            {
                stdinof=1;
                shutdown(sockfd,SHUT_WR); /*send FIN*/
                FD_CLR(fileno(fp),&rset);
                continue;
            }

            written(sockfd,buf,n);
        }
    }
}

#include "unp.h"

int main(int argc, char **argv)
{
    int listenfd, connfd, udpfd, nready, maxfdp1;
    char mesg[MAXLINE];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    const int on=1;
    struct sockaddr_in cliaddr, servaddr;
    void sig_chld(int);

    /*create listening tcp socket */
    listenfd = socket(AF_INET, SOCK_STREAM,0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERV_PORT);

    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on));
    bind(listenfd,(SA*)&servaddr, sizeof(servaddr));

    listen(listenfd,LISTENQ);

    /*create UDP socket*/
    udpfd=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERV_PORT);

    bind(udpfd, (SA *)&servaddr, sizeof(servaddr));

    signal(SIGCHLD,sig_chld); /*must call waitpid() */
    FD_ZERO(&rset);
    maxfdp1=max(listenfd,udpfd)+1;
    for(;;)
    {
        FD_SET(listenfd,&rset);
        FD_SET(udpfd,&rset);

        if( (nready=select(maxfdp1,&rset,NULL,NULL,NULL))<0)
        {
            if(errno==EINTR)
                continue; /*back to for() */
            else
            {
                printf("select error\n");
                return -1;
            }
        }

        if(FD_ISSET(listenfd,&rset))
        {
            len=sizeof(cliaddr);
            connfd=accept(listenfd, (SA *)&cliaddr, &len);

            if( (childpid=fork())==0)
            {
                //child process
                close(listenfd);    /*close listening socket*/
                str_echo(connfd);   //process the request 
                exit(0);
            }

            close(connfd);  //parent closes connected socket
        }

        if(FD_ISSET(udpfd, &rset))
        {
            len=sizeof(cliaddr);
            n=recvfrom(udpfd, mesg, MAXLINE, 0, (SA *)&cliaddr, &len);
            sendto(udpfd,mesg,n, 0, (SA *)&cliaddr,len);
        }
    }
}

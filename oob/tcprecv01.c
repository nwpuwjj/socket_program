#include "unp.h"

int listenfd, connfd;

void sig_urg(int);

int main(int argc, char **argv)
{
    int n;
    char buff[100];

    if(argc == 2)
        listenfd=tcp_listen(NULL,argv[1],NULL);
    else if( argc == 3)
        listenfd = tcp_listen(argv[1],argv[2],NULL);
    else 
        err_quit("usage: tcprecv01 [<host>] <port#>");

    connfd=accept(listenfd,NULL,NULL);

    signal(SIGURG,sig_urg);
    fcntl(connfd, F_SETOWN, getpid());

    for(;;)
    {
        if( (n=read(connfd, buff, sizeof(buff)-1))==0)
        {
            printf("receive EOF\n");
            exit(0);
        }

        buff[n]=0; /*null terminate */
        printf("read %d bytes: %s\n",n,buff);
    }
}

void sig_urg(int signo)
{
    int n;
    char buff[100];

    printf("SIGURG received\n");
    n=recv(connfd,buff,sizeof(buff)-1,MSG_OOB);
    buff[n]=0;      /*null terminate*/
    printf("read %d OOB bytes: %s\n",n, buff);
}


#include "unpthread.h"

void *copyto(void *);

static int sockfd; /* global for both threads to access */
static FILE *fp;

void str_cli(FILE *fp_arg, int sockfd_arg)
{
    char recvline[MAXLINE];
    pthread_t tid;

    sockfd=sockfd_arg; /*copy arguments to externals */
    fp=fp_arg;

    pthread_create(&tid,NULL,copyto, NULL);

    while(readline(sockfd,recvline,MAXLINE)>0)
        fputs(recvline,stdout);
}

void *copyto(void *arg)
{
    char sendline[MAXLINE];

    while(fgets(sendline,MAXLINE,fp)!=NULL)
        writen(sockfd,sendline,strlen(sendline));

    shutdown(sockfd,SHUT_WR); /*EOF on stdin, send FIN */

    return NULL;

    
}

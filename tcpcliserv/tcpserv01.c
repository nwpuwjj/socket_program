#include "unp.h"

ssize_t written(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr=vptr;
    nleft=n;
    while(nleft>0)
    {
        if( (nwritten=write(fd,ptr,nleft))<=0 )
        {
            if(nwritten<0 && errno == EINTR)
                nwritten=0; /*and call write() again */
            else
                return (-1); /*error*/
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return (n);
}


void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];

 again:
    while( (n=read(sockfd,buf,MAXLINE))>0)
        written(sockfd,buf,n);

    if(n<0 && errno==EINTR)
        goto again;
    else if(n<0)
    {
        printf("[wjianjia]===str_echo: read error \n");
    }
}

int main(int argc, char ** argv)
{
    int listenfd, connfd;

    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr,servaddr;

    listenfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERV_PORT);

    bind(listenfd,(SA *) &servaddr,sizeof(servaddr));
    listen(listenfd,LISTENQ);

    for(;;)
    {
        clilen=sizeof(cliaddr);
        connfd=accept(listenfd,(SA *)&cliaddr, &clilen);
        if( (childpid=fork())==0) //child process
        {
            close(listenfd);   //close listening socket
            str_echo(connfd);  //process the request
            exit(0);
        }
        close(connfd);  //parent closes connected socket
    }
}


#include "unp.h"
#include "stddef.h"


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

/* PAINFULLY SLOW VERSION -- example only */
ssize_t readline(int fd, const void *vptr, size_t maxlen)
{
    ssize_t n,rc;
    char c, *ptr;

    ptr=vptr;

    for(n=1;n<maxlen;n++)
    {
again:
        if( (rc=read(fd,&c,1))==1 )
        {
            *ptr++=c;
            if(c=='\n')
                break; /*newline is stored, like fgets()*/
        }
        else if (rc==0)
        {
            *ptr = 0;
            return(n-1); /*EOF, n-1 bytes were read */
        }
        else
        {
            if(errno==EINTR)
            {
                goto again;
            }

            return (-1);  /*error, errno set by read()*/
        }
    }

    *ptr=0;
    return (n);
}


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


int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 2)
    {
        printf("[wjianjia]==usage: tcpcli <IPaddress>");
        return -1;
    }

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
    connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    str_cli(stdin,sockfd); /* do it all*/
    exit(0);
}

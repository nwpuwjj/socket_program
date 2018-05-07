#include "unp.h"

int main(int argc, char **argv)
{
    printf("==[wjianjia]===hello 1\n");
    int sockfd,n;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;

    if(argc != 2)
    {
        err_quit("usage: a.out <IPADDRESS>");
    }
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        err_quit("socket error");
    }

    printf("==[wjianjia]=socket success\n");
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(13);    /*daytime server */
    if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0)
    {   
        err_quit("inet_pton error for %s \n", argv[1]);
    }

    printf("[wjianjia]===inet_pton success for %s\n",argv[1]);

    if(connect(sockfd,(SA *)&servaddr, sizeof(servaddr))<0)
    {
        err_sys("connect error");
    }


    printf("[wjianjia]===connect success \n");

    while( (n=read(sockfd,recvline,MAXLINE))>0)
    {
        recvline[n]=0; /*null terminate */
        if(fputs(recvline,stdout)==EOF)
        {
            err_sys("fputs error");
        }
    }

    if(n<0)
    {
        err_sys("read error");
    }

    exit(0);
}

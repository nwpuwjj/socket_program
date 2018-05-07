#include "unp.h"
#include <time.h>

int main(int argc, char **argv)
{
    printf("==wjianjia ==log 1\n");
    int listenfd,connfd;
    struct sockaddr_in servaddr;

    char buff[MAXLINE];
    time_t ticks;

    listenfd=socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(13); /*daytime server */

    if(bind(listenfd,(SA *)&servaddr, sizeof(servaddr))<0)
    {
        printf("[wjianjia]===bind fail\n");
        return -1;
    }
    printf("[wjianjia]==bind success\n");
    if(listen(listenfd,LISTENQ)<0)
    {
        printf("[wjianjia] ==== listen fail\n");
        return -1;
    }
    printf("[wjianjia]===listen success\n");

    for(;;)
    {
        connfd=accept(listenfd,(SA *)NULL, NULL);
        printf("===[wjianjia]===client connected in===\n");
        ticks =time(NULL);
        snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
        printf("===[wjianjia]==client buff:%s===\n",buff);
        write(connfd,buff,strlen(buff));
        close(connfd);
    }
}

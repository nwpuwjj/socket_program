#include "unp.h"

#define NDG 2000 /*datagrams to send */
#define DGLEN 1400 /*length of each datagram */

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int i;
    char sendline[DGLEN];

    for(i=0;i<NDG;i++)
    {
        sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen);
    }
}


int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc !=2)
    {
        printf("usage: udpcli <IPaddress>\n");
        return -1;
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr);

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    dg_cli(stdin,sockfd,(SA*)&servaddr,sizeof(servaddr));

    exit(0);
}


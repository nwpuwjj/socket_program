#include "unp.h"
#define SAP_NAME "sap.mcast.net"  /*default group name and port*/
#define SAP_PORT "9875"

void loop(int, socklen_t);

int main(int argc, char **argv)
{
    int sockfd;
    const int on=1;
    socklen_t salen;
    struct sockaddr *sa;
    if(argc ==1)
        sockfd = udp_client(SAP_NAME,SAP_PORT,(void **)&sa, &salen);
    else if(argc == 4)
        err_quit("usage: mysdr <mcast-addr> <port#> <interface-name>");
    setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bind(sockfd,sa,salen);
    mcast_join(sockfd,sa,salen,(argc==4)?argv[3]:NULL,0);
    loop(sockfd,salen); /*receive and print*/
    exit(0);
}

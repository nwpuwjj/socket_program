#include "unp.h"

void recv_all(int,socklen_t);
void send_all(int,SA *,socklen_t);

int main(int argc, char **argv)
{
    int sendfd, recvfd;
    const int on=1;
    socklen_t salen;
    struct sockaddr *sasend, *sarecv;
    if(argc!=3)
        err_quit("usage: sendrecv <IP-multicast-address> <port#>");
    sendfd=udp_client(argv[1],argv[2],(void **)&sasend, &salen);
    recvfd=socket(sasend->sa_family, SOCK_DGRAM,0);
    setsockopt(recvfd,SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on));
    sarecv=malloc(salen);
    memcpy(sarecv,sasend,salen);
    bind(recvfd,sarecv,salen);
    mcast_join(recvfd,sasend,salen,NULL,0);
    mcast_set_loop(sendfd,0);
    if(fork()==0)
        recv_all(recvfd,salen); /*child->receives*/
    send_all(sendfd,sasend,salen); /*parent->sends*/
}

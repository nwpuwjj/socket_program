#include "udpcksum.h"

/* Define global variables*/
struct sockaddr *dest, *local;
struct sockaddr_in locallookup;
socklen_t destlen, locallen;

int datalink;       /*from pcap_datalink(),in <net/bpf.h>*/
char *device;       /*pcap device*/
pcap_t *pd;         /*packet capture struct pointer*/
int rawfd;          /*raw socket to write on*/
int snaplen=200;    /*amount of data to capture*/
int verbose;
int zerosum;        /*send UDP query with no checksum*/

static void usage(const char *);

int main(int argc, char* argv[])
{
    int c,lopt=0;
    char *ptr, localname[1024], *localport;
    struct addrinfo *aip;
    opterr=0;       /*don't want getopt() writing to stderr */
    while( (c=getopt(argc, argv, "0i:l:v"))!=-1)
    //冒号表示参数，一个冒号就表示这个选项后面必须带有参数，两个冒号的表示这个选项是可选的。
    {
        switch(c){
            case '0':
                zerosum=1;
                break;
            case 'i':
                device=optarg;  /*pcap device*/
                break;
            case 'l':           /*local IP address and port #:a.b.c.d.p */
                if((ptr=strrchr(optarg,'.'))==NULL)
                    usage("invalid -l option");
                *ptr++=0;   /*null replaces final period*/
                localport=ptr;  /*service name or port number*/
                strncpy(localname,optarg,sizeof(localname));
                lopt=1;
                break;

           case 'v':
                verbose=1;
                break;

           case '?':  //getopt 输出自己的出错信息，并且返回’？‘
                usage("unrecognized option");
        }                
    }

    if(optind != argc-2)
        usage("missing <host> and/or <serv>");

    /*convert destination name and service*/
    aip=host_serv(argv[optind],argv[optind+1],AF_INET,SOCK_DGRAM);
    dest=aip->ai_addr;  /*don't freeaddrinfo()*/
    destlen=aip->ai_addrlen;

    /*
        need local ip address for source ip address for udp datagrams.
        can't specify 0 and let IP choose, as we need to know it for the 
        pseudoheader to calculate the udp checksum. if -l option supplied, 
        then use those values; otherwise, connect a udp socket to the destination
        to determine the right source address.
    */
    if(lopt){
        /*convert local name and service*/
        aip=host_serv(localname,localport,AF_INET,SOCK_DGRAM);
        local=aip->ai_addr;     /*don't freeaddrinfo()*/
        locallen=aip->ai_addrlen;
    }
    else
    {
        int s;
        s=socket(AF_INET,SOCK_DGRAM,0);
        connect(s,dest,destlen);
        /*kernel chooses correct local address for dest */
        locallen= sizeof(locallookup);
        local=(struct sockaddr *)&locallookup;
        getsockname(s,local,&locallen);

        if(locallookup.sin_addr.s_addr == htonl(INADDR_ANY))
            err_quit("can't determine local address - use -l\n");
        close(s);
    }

    /*调用open_output 函数创建一个原始套接字并开启IP_HDRINCL套接字选项，我们于是可以往这个套接字写出包括IP首部在内的完整IP数据包*/
    open_output();  /*open output, either raw socket or libnet*/
    
    open_pcap();    /*open packet capture device*/
    setuid(getuid());   /*don't need superuser privileges anymore*/

    signal(SIGTERM, cleanup);
    signal(SIGINT, cleanup);
    signal(SIGHUP,cleanup);

    test_udp();
    cleanup(0);
}
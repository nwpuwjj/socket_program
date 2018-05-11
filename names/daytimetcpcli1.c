#include "unp.h"

char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char portstr[8];
    static char str[128];   /*unix domain is largest */

    switch(sa->sa_family)
    {
        case AF_INET: {
            struct sockaddr_in *sin= (struct sockaddr_in *) sa;

            if(inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str))==NULL)
                return NULL;
            if(ntohs(sin->sin_port)!=0)
            {
                snprintf(portstr, sizeof(portstr),":%d", ntohs(sin->sin_port));
                strcat(str,portstr);
            }

            return(str);
        }
    }
}


int main(int argc, char ** argv)
{
    int sockfd, n;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;
    struct in_addr **pptr;
    struct in_addr *inetaddrp[2];
    struct in_addr inetaddr;
    struct hostent *hp;
    struct servent *sp;
    printf("==wjianjia==\n");

    if(argc !=3)
    {
        printf("usage: daytimetcpcli1 <hostname> <service>\n");
        return -1;
    }

    if( (hp=gethostbyname(argv[1]))==NULL)
    {
        if(inet_aton(argv[1],&inetaddr)==0)
        {
            printf("hostname error for %s: %s\n",argv[1], hstrerror(h_errno));
            return -1;
        }else
        {
            inetaddrp[0]=&inetaddr;
            inetaddrp[1]=NULL;
            pptr=inetaddrp;
        }
    }
    else
    {
        pptr=(struct in_addr **) hp->h_addr_list;
    }

    if( (sp=getservbyname(argv[2],"tcp"))==NULL)
    {
        printf("getservbyname error for %s\n", argv[2]);
        return -1;
    }

    for(; *pptr!=NULL; pptr++)
    {
        
        sockfd=socket(AF_INET,SOCK_STREAM,0);
        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family=AF_INET;
        servaddr.sin_port=sp->s_port;
        memcpy(&servaddr.sin_addr,*pptr,sizeof(struct in_addr));

        printf("trying %s\n", sock_ntop((SA *)&servaddr, sizeof(servaddr)));

        if(connect(sockfd, (SA *)&servaddr, sizeof(servaddr))==0)
        {
            break; //success
        }
        printf("connect error\n");
        close(sockfd);
    }

    if(*pptr == NULL)
    {
        printf("unable to connect \n");
        return -1;
    }
    printf("connect success\n");
    while((n=read(sockfd,recvline, MAXLINE))>0)
    {
        recvline[n]=0;  /*null terminate */
        printf("n:%d==\n",n);
        printf("recvline:%s\n",recvline);
        fputs(recvline,stdout);
    }

    exit(0);
}

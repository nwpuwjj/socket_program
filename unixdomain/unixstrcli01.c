#include "unp.h"
int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_un servaddr;
    sockfd = socket(AF_LOCAL,SOCK_STREAM,0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

    str_cli(stdin,sockfd); /*do it all */

    exit(0);
}

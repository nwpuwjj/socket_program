#include "unp.h"
#include <time.h>

char *gf_time(void)
{
    struct timeval tv;
    static char str[30];
    char *ptr;

    if(gettimeofday(&tv,NULL)<0)
    {
        printf("gettimeofday error\n");
    }

    ptr=ctime(&tv.tv_sec);
    strcpy(str,&ptr[11]);
    /* Fri Sep 13 00:00::00 1986\n\0 */
    snprintf(str+8，sizeof(str)-8,".%06ld",tv.tv_unsec);
    return str;
}

void str_cli(FILE *fp, int sockfd)
{
    int maxfdp1, va, stdineof;
    ssize_t n, nwritten;
    fd_set rset, wset;
    char to[MAXLINE], fr[MAXLINE];
    char *toiptr, *tooptr, *friptr, *froptr;

    val=fcntl(sockfd, F_GETFL,0);
    fcntl(sockfd,F_SETFL,val|O_NONBLOCK);

    val=fcntl(STDIN_FILENO,F_GETFL,0);
    fcntl(STDIN_FILENO,F_SETFL,val|O_NONBLOCK);

    val=fcntl(STDOUT_FILENO,F_GETFL,0);
    fcntl(STDOUT_FILENO,F_SETFL,val|O_NONBLOCK);

    toiptr=tooptr=to; /*initialize buffer pointers*/
    friptr=froptr=fr;
    stdineof=0;

    maxfdp1=max(max(STDIN_FILENO,STDOUT_FILENO),sockfd)+1;

    for(;;)
    {
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        if(stdineof==0 && toiptr < &to[MAXLINE])
            FD_SET(STDIN_FILENO, &rset); /*read from stdin*/
        if(friptr < &fr[MAXLINE])
            FD_SET(sockfd,&rset);  /*read from socket */
        if(tooptr != toiptr)
            FD_SET(sockfd,&wset); /*data to write to socket*/
        if(froptr!=friptr)
            FD_SET(STDOUT_FILENO,&wset); /*data to write to stdout*/

        selec(maxfdp1, &rset, &wset, NULL,NULL);

        if(FD_ISSET(STDIN_FILENO, &rset))
        {
            if( (n=read(STDIN_FILENO, toiptr, &to[MAXLINE]-toiptr))<0)
            {
                if(errno !=EWOULDBLOCK)
                {
                    printf("read error on stdin\n");
                    return ;
                }
            }
            else if(n==0)
            {
                fprintf(stderr,"%s: EOF on stdin\n",gf_time());
                stdineof=1; /*all done with stdin*/
                if(tooptr==toiptr)
                {
                    shutdown(sockfd,SHUT_WR); /*send FIN */
                }
           }
           else
           {
                fprintf(stderr,"%s: read %d bytes from stdin\n",gf_time(),n);
                toiptr +=n;  /*#just read*/
                FD_SET(sockfd,&wset); /*try and write to socket below*/
            }
        }

        if(FD_ISSET(sockfd, &rset))
        {
            if( (n=read(sockfd,friptr, &fr[MAXLINE]-friptr)) <0)
            {
                if(errno != EWOULDBLOCK)
                {
                    printf("read error on socket\n");
                    return;
                }
            }
            else if(n==0)
            {
                fprintf(stderr, "%s: EOF on socket\n",gf_time());
                if(stdineof)
                    return;  /*normal termination*/
                else
                {
                    printf("str_cli: server terminated prematurely\n");
                    return;
                }
            }
            else
            {
                fprintf(stderr,"%s: read %d bytes from socket\n", gf_time(),n);
                friptr +=n; /* #just read */
                FD_SET(STDOUT_FILENO,&wset); /*try and write below*/
                
            }
        }

        if(FD_ISSET(STDOUT_FILENO, &wset) && ( (n=friptr-froptr)>0))
        {
            if( (nwritten = write(STDOUT_FILENO, froptr, n))<0)
            {
                if(errno != EWOULDBLOCK)
                {
                    printf("write error to stdout\n");
                    return;
                }
            }
            else
            {
                fprintf(stderr,"%s:wrote %d bytes to stdout\n",gf_time(),nwritten);
                froptr+=nwritten; /*#just written*/
                if(froptr==friptr)
                    froptr=friptr=fr; /*back to beginning of buffer*/
            }
        }

        if(FD_ISSET(sockfd,&wset) && ( (n=toiptr-tooptr)>0))
        {
            if( (nwritten = write(sockfd,tooptr,n))<0)
            {
                if(errno != EWOULDBLOCK)
                {
                    printf("write error to socket\n");
                    return;
                }
            }
            else
            {
                fprintf(stderr, "%s, wrote %d bytes to socket \n",gf_time(),nwritten);
                tooptr+=nwritten;/*#just written*/

                if(tooptr==toiptr)
                {
                    toiptr=tooptr=to; /*back to begining of buffer*/
                    if(stdineof)
                        shutdown(sockfd,SHUT_WR);  /*send FIN */
                }
            }
        }
       
          
    }
}

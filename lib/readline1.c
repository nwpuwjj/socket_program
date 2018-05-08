#include "unp.h"

/* PAINFULLY SLOW VERSION -- example only */
ssize_t written(int fd, const void *ptr, size_t n)
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

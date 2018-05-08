#include "unp.h"

/*

#include <unistd.h>    
ssize_t read(int fd, void *buf, size_t count);  
返回值：成功返回读取的字节数，出错返回-1并设置errno，如果在调read之前已到达文件末尾，则这次read返回0

*/



// read "n" bytes from a descriptor. 
ssize_t readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr=vptr;
    nleft=n;
    while(nleft >0)
    {
        if( (nread=read(fd,ptr,nleft))<0)
        {
            if(errno==EINTR)
            {
                nread=0;  /*and call read() again*/
            }
            else
            {
                return -1;
            }
        }
        else if (nread == 0)
            break;  /*EOF*/

        nleft -=nread;
        ptr +=nread;
    }

    return (n-nleftn);
}

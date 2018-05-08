#include "unp.h"

static int read_cnt;  //缓冲区中待读取的字节数
static char *read_ptr;  //缓冲区读指针
static char read_buf[MAXLINE]; //指定大小的缓冲区

static ssize_t my_read(int fd, char *ptr)
{
    if(read_cnt<=0)
    {
    again:
        //read 碰到换行符也会返回
        if( (read_cnt = read(fd,read_buf, sizeof(read_buf))) <0 )
        {
            if(errno == EINTR)
                goto again;

            return -1;
        }
        else if(read_cnt == 0)
            return 0;

        read_ptr=read_buf;
    }

    read_cnt--;
    *ptr= *read_ptr++;
    return 1;
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n,rc;
    char c,*ptr;

    ptr=vptr;
    for(n=1;n<maxlen;n++)
    {
        if((rc=my_read(fd,&c))==1)
        {
            *ptr++=c;
            if(c=='\n')
                break;  /*newline is stored,like fgets()*/
        }
        else if (rc==0)
        {
            *ptr=0;
            return (n-1); /*EOF,n-1 bytes were read */
        }
        else
            return -1;  /*error, errno set by read() */
    }

    *ptr=0;
    return n;
}

//读取缓冲区
ssize_t readlinebuf(void **vptrptr)
{
    if(read_cnt)
    {
        *vptrptr=read_ptr;
    }
    return (read_cnt);
}

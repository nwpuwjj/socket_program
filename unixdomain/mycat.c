#include "unp.h"

int my_open(const char *,int);

int main(int argc, char ** argv)
{
    int fd,n;
    char buff[BUFFSIZE];

    if(argc!=2)
    {
        printf("usage: mycat <pathname>\n");
        return -1;
    }

    if( (fd=my_open(argv[1],O_RDONLY))<0)
    {
        printf("cannot open %s\n",argv[1]);
        return -1;
    }

    while( (n=read(fd,buff,BUFFSIZE))>0)
        write(STDOUT_FILENO,buff,n);

    exit(0);
}

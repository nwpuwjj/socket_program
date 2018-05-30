#include "udpcksum.h"
#include <setjmp.h>

static sigjmp_buf jmpbuf;
static int canjump;

void sig_alrm(int signo)
{
    if(canjump==0)
        return;
    siglongjmp(jmpbuf,1);
}

void test_udp(void)
{
    /*
    一个定义为volatile的变量是说这变量可能会被意想不到地改变，这样，编译器就不会假设这个变量的值了，精确地说就是，优化器在用到
    这个变量时必须每次都小心地重新读取这个变量的值，而不是使用保存在寄存器里的备份。下面是volatile变量的几个例子：
    1.并行设备的硬件寄存器（如：状态寄存器）
    2.一个中断服务子程序中会访问到的非自动变量（non-automatic variables)
    3.多线程应用中被几个任务共享的变量
    */
    volatile int nsent=0, timeout=3;
    struct udpiphdr *ui;

    signal(SIGALRM,sig_alrm);

    if(sigsetjmp(jmpbuf,1))
    {
        if(nsent>=3)
            err_quit("no response");
        printf("timeout\n");
        timeout *=2;    /*exponential backoff:3,6,12*/
    }

    canjump=1;      /*siglongjmp is now ok*/

    send_dns_query();
    nsent++;

    alarm(timeout);
    ui=udp_read();
    canjump=0;
    alarm(0);

    if(ui->ui_sum==0)
        printf("UDP checksums off\n");
    else
        printf("UDP checksums on\n");
    if(verbose)
        printf("received UDP checksum = %x\n",ntohs(ui->ui_sum));
}

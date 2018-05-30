//cleanup函数，由main函数在程序即将终止时调用，同时也是用于中断程序的那些键盘输入信号的信号处理函数
void cleanup(int signo)
{
    struct pcap_stat stat;
    putc('\n',stdout);

    if(verbose)
    {
        //使用pcap_stats 获取分组捕获统计信息：由过滤器接收的分组总数以及由内核丢弃的分组总数
        if(pcap_stats(pd,&stat)<0)
            err_quit("pcap_stats: %s\n",pcap_geterr(pd));
        printf("%d packets received by filter\n",stat.ps_recv);
        printf("%d packets dropped by kernel\n",stat.ps_drop);
    }

    exit(0);
}

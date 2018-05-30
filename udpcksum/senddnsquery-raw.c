
//构造一个DNS查询，并通过原始套接字把该UDP数据报发送给名字服务器
void send_dns_query(void)
{
    size_t nbytes;
    char *buf, *ptr;

    //使用malloc 分配缓冲区buf,它足以存放20字节的IP首部，8字节的UDP首部以及100字节的用户数据。
    //并把指针ptr初始化为指向用户数据的第一个字节
    buf=malloc(sizeof(struct udpiphdr)+100);
    ptr=buf+sizeof(struct udpiphdr);    /*leave room for IP/UDP headers*/

    *((uint16_t *)ptr)=htons(1234); /*identification*/
    ptr+=2;
    *((uint16_t *)ptr)=htons(0x0100); /*flags: recursion desired*/
    ptr+=2;
    *((uint16_t *)ptr)=htons(1);    /*#questions */
    ptr+=2;
    *((uint16_t *)ptr)=0; /*#answer RRs*/
    ptr+=2;
    *((uint16_t *)ptr)=0; /*#authority RRs */
    ptr+=2;
    *((uint16_t *)ptr)=0; /*#additional RRs */
    ptr+=2;

    memcpy(ptr,"\001a\014root-servers\003net\000",20);
    ptr+=20;
    *((uint16_t*)ptr)=htons(1); /*query type = A */
    ptr+=2;
    *((uint16_t *)ptr)=htons(1); /*query class =1 (IP addr) */
    ptr+=2;

    nbytes = (ptr-buf)-sizeof(struct udpiphdr);
    udp_write(buf,nbytes);
    if(verbose)
        printf("sent:%d bytes of data\n",nbytes);
}

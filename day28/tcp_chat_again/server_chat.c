#include <func.h>

int main(int argc, char** argv)
{
    //内核里端口和ip地址存储的是网络字节序
    ARGS_CHECK(argc,3);
    int ret, sfd;
    //socket
    sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd,-1,"socket");
    //bind
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET; //协议族ipv4进行通信
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); //点分十进制转为网络字节序
    server_addr.sin_port = htons(atoi(argv[2])); //端口转换为网络字节序：大端
    
    ret = bind(sfd, (struct sockaddr *)&server_addr,sizeof(server_addr));
    ERROR_CHECK(ret,-1,"bind");
    //listen
    ret = listen(sfd, 10);
    ERROR_CHECK(ret,-1,"listen");
    //accept
    int newfd;
    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr));
    socklen_t len = sizeof(client_addr);

    //select轮询检查可读的文件(缓冲区)
    char buf[128]={0};
    fd_set rdset,tmpset; //tmpset作为传入传出参数
    FD_ZERO(&rdset);
    FD_SET(STDIN_FILENO,&rdset);
    FD_SET(sfd,&rdset);
    while(1)
    {
        memcpy(&tmpset,&rdset,sizeof(rdset));
        ret = select(11,&tmpset,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
        if(FD_ISSET(sfd,&tmpset))
        {
            newfd = accept(sfd,(struct sockaddr *)&client_addr,&len);
            ERROR_CHECK(newfd,-1,"accept");
            FD_SET(newfd,&rdset);
            printf("client %s:%d has connected server!\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port) );
        }
        if (FD_ISSET(STDIN_FILENO, &tmpset))
        {
            bzero(buf,sizeof(buf));
            ret = read(STDIN_FILENO,buf,sizeof(buf));
            ERROR_CHECK(ret,-1,"read");
            if(0 == ret)
            {
                printf("byebye!\n");
                break;
            }
            ret = send(newfd,buf,strlen(buf)-1,0);  //发送除了\n的标准输入
            ERROR_CHECK(ret,-1,"send");
            // printf("server:\n%s",buf);
        }
        if (FD_ISSET(newfd, &tmpset))
        {
            bzero(buf,sizeof(buf));
            ret = recv(newfd,buf,sizeof(buf),0);
            if(0 == ret) //如果客户端断开
            {
                printf("byebye!\n");
                FD_CLR(newfd,&rdset);
            } 
            else
            {
                printf("                   client:\n                   \t%s\n",buf);
            }
        }
    }
    
    close(newfd);
    close(sfd);
    return 0;
}
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

    newfd = accept(sfd,(struct sockaddr *)&client_addr, &len);
    ERROR_CHECK(newfd,-1,"accept");
    printf("client %s:%d has connected server!\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port) );
    //select轮询检查可读的文件(缓冲区)
    fd_set rdset;
    char buf[128]={0};
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(newfd,&rdset);
        ret = select(newfd+1,&rdset,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
        if (FD_ISSET(STDIN_FILENO, &rdset))
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
        if (FD_ISSET(newfd, &rdset))
        {
            bzero(buf,sizeof(buf));
            ret = recv(newfd,buf,sizeof(buf),0);
            if(0 == ret)
            {
                printf("byebye!\n");
                break;
            }
            printf("                   client:\n                   \t%s\n",buf);
        }
    }
    
    close(newfd);
    close(sfd);
    return 0;
}
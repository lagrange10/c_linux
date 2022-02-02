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
    //recv
    char buf[128]={0};
    ret = recv(newfd,buf,sizeof(buf),0);
    ERROR_CHECK(ret,-1,"recv");
    printf("i am server, gets %s\n",buf);
    
    close(newfd);
    close(sfd);
    return 0;
}
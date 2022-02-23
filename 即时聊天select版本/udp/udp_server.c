#include <func.h>

int main(int argc,char **argv)
{
    ARGS_CHECK(argc,3);
    int ret, sfd;
    //定义一个udp
    sfd = socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sfd,-1,"socket");

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    ret = bind(sfd,(struct sockaddr *)&server_addr, sizeof(server_addr));
    ERROR_CHECK(ret,-1,"bind");
    //bind后就已经处于”监听“状态了
    char buf[128]={0};
    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr));
    socklen_t len = sizeof(client_addr); //要初始化len,传入传出参数，是缓冲区大小的预分配值和实际值
    ret = recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&len);
    printf("client %s:%d has connected!\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port) );
    printf("gets %s\n", buf);
    ret = sendto(sfd,"hi",2,0,(struct sockaddr *)&client_addr, sizeof(client_addr));
    ERROR_CHECK(ret,-1,"sendto");

    //即时聊天
    fd_set rd_set;
    while(1)
    {
        FD_ZERO(&rd_set);
        FD_SET(STDIN_FILENO,&rd_set);
        FD_SET(sfd,&rd_set);
        ret = select(sfd+1, &rd_set,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select");
        if (FD_ISSET(STDIN_FILENO, &rd_set))
        {
            bzero(buf,sizeof(buf));
            ret = read(STDIN_FILENO,buf,sizeof(buf));
            ERROR_CHECK(ret,-1,"read");
            ret = sendto(sfd, buf, strlen(buf)-1,0,(struct sockaddr *)&client_addr, len);
            ERROR_CHECK(ret,-1,"sendto");
        }
        if (FD_ISSET(sfd, &rd_set))
        {
            bzero(buf,sizeof(buf));
            ret = recvfrom(sfd,buf,sizeof(buf),0, (struct sockaddr *)&client_addr, &len);
            printf("%s\n",buf);
        }
    }
    

    
    return 0;
}
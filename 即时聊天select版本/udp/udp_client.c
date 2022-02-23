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
    ret = sendto(sfd,"hello",5,0,(struct sockaddr *)&server_addr, sizeof(server_addr));
    ERROR_CHECK(ret,-1,"sendto");
    socklen_t len;
    char buf[128]={0};
    ret = recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr *)&server_addr,&len);
    ERROR_CHECK(ret,-1,"recvfrom");
    printf("i am client, gets %s\n",buf);
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
            ret = sendto(sfd, buf, strlen(buf)-1,0,(struct sockaddr *)&server_addr, len);
            ERROR_CHECK(ret,-1,"sendto");
        }
        if (FD_ISSET(sfd, &rd_set))
        {
            bzero(buf,sizeof(buf));
            ret = recvfrom(sfd,buf,sizeof(buf),0, (struct sockaddr *)&server_addr, &len);
            printf("%s\n",buf);
        }
    }
    return 0;
}
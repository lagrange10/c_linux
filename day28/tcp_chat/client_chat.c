#include <func.h>

int main(int argc, char** argv)
{
    ARGS_CHECK(argc,3);
    int ret, sfd;
    //socket
    sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd,-1,"socket");

    //connect
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof(server_addr);

    ret = connect(sfd,(struct sockaddr *)&server_addr, len);
    ERROR_CHECK(sfd,-1,"connect");
    printf("connect %s:%d successfully\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));
        //select轮询检查可读的文件(缓冲区)
    fd_set rdset;
    char buf[128]={0};
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(sfd,&rdset);
        ret = select(sfd+1,&rdset,NULL,NULL,NULL);
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
            ret = send(sfd,buf,strlen(buf)-1,0);  //发送除了\n的标准输入
            // printf("server:\n%s",buf);
            ERROR_CHECK(ret,-1,"send");
        }
        if (FD_ISSET(sfd, &rdset))
        {
            //bzero写错导致每次都多打了一些东西
            bzero(buf,sizeof(buf));
            ret = recv(sfd,buf,sizeof(buf),0);
            if(0 == ret)
            {
                printf("byebye!\n");
                break;
            }
            printf("                   server:\n                   \t%s\n",buf);
        }
    }

    close(sfd);
    return 0;
}
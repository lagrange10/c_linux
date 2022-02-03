#include <func.h>

int fcntl_nonblock(int fd)
{
    int status = fcntl(fd,F_GETFL);
    status |= FNONBLOCK;
    int ret = fcntl(fd,F_SETFL,status);
    ERROR_CHECK(ret,-1,"fcntl");
    return 0;
}


int main(int argc, char** argv)
{
    //内核里端口和ip地址存储的是网络字节序
    ARGS_CHECK(argc,3);
    int ret, sfd;
    //socket
    sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd,-1,"socket");
    //设置成可重用端口
    int reuse=1;
    ret = setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,4);
    ERROR_CHECK(ret,-1,"setsockopt");
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
    //设置newfd为非阻塞读
    fcntl_nonblock(newfd);
    //epoll轮询检查可读的文件(缓冲区)
    int epfd = epoll_create(1);
    struct epoll_event event,evs[2];
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN; //监视是否可读
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl0");
    event.data.fd = newfd;
    event.events = EPOLLIN|EPOLLET; //监视是否可读,边沿触发edge trigger
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,newfd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl1");

    int ready_fd_num;
    char buf[6]={0};
    while(1)
    {
        //evs是传出参数，根据每次读成功的数量去写evs数组。
        //例如2个事件有一个发生那么它会存在evs[0]？
        ready_fd_num = epoll_wait(epfd,evs,2,-1);
        for(int i=0;i<ready_fd_num;i++)
        {
            if (evs[i].data.fd == STDIN_FILENO)
            {
                bzero(buf,sizeof(buf));
                ret = read(STDIN_FILENO,buf,sizeof(buf)-1);
                ERROR_CHECK(ret,-1,"read");
                if(0 == ret)
                {
                    printf("byebye!\n");
                    return 0;
                }
                ret = send(newfd,buf,strlen(buf)-1,0);  //发送除了\n的标准输入
                ERROR_CHECK(ret,-1,"send");
                // printf("server:\n%s",buf);
            }
            if (evs[i].data.fd == newfd)
            {
                printf("                   client:\n                   \t");
                while(1)
                {
                    bzero(buf,sizeof(buf));
                    ret = recv(newfd,buf,sizeof(buf)-1,0);
                    if(0 == ret)
                    {
                        printf("byebye!\n");
                        return 0;
                    } else if (-1 == ret)
                    {
                        printf("\n");
                        break;
                    } else
                    {
                        printf("%s",buf);
                    }
                }
                
            }
        }
    }
    
    close(newfd);
    close(sfd);
    return 0;
}
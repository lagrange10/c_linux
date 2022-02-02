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
    //send
    ret = send(sfd,"hello",5,0);
    ERROR_CHECK(ret,-1,"send");

    close(sfd);
    return 0;
}
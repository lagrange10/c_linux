#include <func.h>
//chat2 写1号，读2号
int main(int argc,char* argv[])
{
	ARGS_CHECK(argc,3);
    int fdr,fdw;
    fdw = open(argv[1],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open1");
    fdr = open(argv[2],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open2");
    printf("i am chat2 process!\n");
    char buf[128];
    //先接收数据,再发送
    while(1)
    {
        memset(buf,0,sizeof(buf));
        read(fdr,buf,sizeof(buf));
        printf("get:%s\n",buf);
        memset(buf,0,sizeof(buf));
        read(STDIN_FILENO, buf,sizeof(buf));
        write(fdw,buf,strlen(buf)-1);
    }
    close(fdr);
    close(fdw);
    return 0;
}
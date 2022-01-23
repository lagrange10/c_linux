#include <func.h>
//chat1 写2号，读1号
int main(int argc,char* argv[])
{
	ARGS_CHECK(argc,3);
    int fdr,fdw;
    fdr = open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open1");
    fdw = open(argv[2],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open2");
    printf("i am chat1 process!\n");
    char buf[128];
    //先发送数据,再读
    while(1)
    {
        memset(buf,0,sizeof(buf));
        read(STDIN_FILENO, buf,sizeof(buf));
        write(fdw,buf,strlen(buf)-1);
        memset(buf,0,sizeof(buf));
        read(fdr,buf,sizeof(buf));
        printf("get:%s\n",buf);
    }
    close(fdr);
    close(fdw);
    return 0;
}
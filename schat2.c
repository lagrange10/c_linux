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
    fd_set rdset;
    int ret;
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(fdr,&rdset);
        select(fdr+1,&rdset,NULL,NULL,NULL);
        if (FD_ISSET(STDIN_FILENO,&rdset))
        {
            memset(buf,0,sizeof(buf));
            ret = read(STDIN_FILENO, buf,sizeof(buf));
            if (0 == ret)
            {
                puts("i want to leave, bye!");
                break;
            }
            write(fdw,buf,strlen(buf)-1);
        }
        if (FD_ISSET(fdr,&rdset))
        {
            memset(buf,0,sizeof(buf));
            ret = read(fdr,buf,sizeof(buf));
            if (0 == ret)
            {
                puts("bye!");
                break;
            }
            printf("giao1:%s\n",buf);
        }
    }
    close(fdr);
    close(fdw);
    return 0;
}
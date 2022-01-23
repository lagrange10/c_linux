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
    fd_set rdset;
    int ret;
    int ready_pnum;
    struct timeval timeout;
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(fdr,&rdset);
        timeout.tv_sec=3;
        ready_pnum = select(fdr+1,&rdset,NULL,NULL,&timeout);
        if (ready_pnum > 0)
        {
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
                printf("giao2:%s\n",buf);
            }
        }
        else
        {
            printf("do someting\n");
        }
        
    }
    close(fdr);
    close(fdw);
    return 0;
}
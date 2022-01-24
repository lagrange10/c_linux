#include <func.h>

int main()
{
    int fds[2],ret;
    //0=r , 1=w;
    pipe(fds);
    if (fork())
    {
        close(fds[1]);
        char buf[128]={0};
        ret = read(fds[0],buf,sizeof(buf));
        ERROR_CHECK(ret,-1,"read");
        printf("i am parent, %s\n",buf);
        return 0;
    } else
    {
        close(fds[0]);
        ret = write(fds[1],"Hello",5);
        ERROR_CHECK(ret,-1,"write");
        return 0;
    }
}
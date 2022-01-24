#include <func.h>

int Daemon()
{
    if(fork())
    {
        exit(0);
    } 
    else
    {
        setsid();
        chdir("/");
        umask(0);
        for(int i=0;i<3;i++) close(i);
    }
}

int main()
{
    Daemon();
    int fd;
    char buf[]="wo hen shuai! ";
    fd=open("/tmp/log",O_RDWR|O_CREAT,0666);
    while(1)
    {
        lseek(fd,0,SEEK_END);
        write(fd,buf,strlen(buf));
        sleep(2);
    }
    return 0;
}
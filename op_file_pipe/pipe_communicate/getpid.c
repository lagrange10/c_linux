#include <func.h>

int main()
{
    printf("pid=%d, ppid=%d, user=%d, group=%d, euid=%d, egid=%d\n", getpid(), getppid()\
    , getuid(), getgid(), geteuid(), getegid());
    while(1);  //测试进程用
    return 0;
}
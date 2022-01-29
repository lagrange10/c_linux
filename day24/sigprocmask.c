#include <func.h>

void sigfunc(int signum, siginfo_t *pinfo, void * pReversed)
{
    printf("before sleep, sig%d is coming\n", signum);
    sleep(3);
    
    printf("after sleep, sig%d is coming\n", signum);
}

int main()
{
    struct sigaction act;
    bzero(&act,sizeof(act));
    act.sa_flags = SA_SIGINFO; //新类型
    act.sa_sigaction = sigfunc;
    //设置信号行为
    int ret = sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret,-1,"sigaction");
    //进入关键代码段之前，阻塞
    //屏蔽2号信号
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    //关键代码段
    sleep(5);
    //把内核里面的sigpending取出来
    sigset_t pending;
    sigemptyset(&pending);
    sigpending(&pending);
    if (sigismember(&pending,SIGINT))
    {
        printf("sig 2 SIGINT is pending!\n");
    }
    else
    {
        printf("sig 2 SIGINT is not pending!\n");
    }
    //解除阻塞
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);


    return 0;
}
#include <func.h>

void sigfunc(int signum, siginfo_t *pinfo, void * pReversed)
{
    printf("before sleep, sig%d is coming\n", signum);
    sleep(3);
    //把内核里面的sigpending取出来
    sigset_t pending;
    sigemptyset(&pending);
    sigpending(&pending);
    if (sigismember(&pending,SIGQUIT))
    {
        printf("sig 3 SIGQUIT is pending!\n");
    }
    else
    {
        printf("sig 3 SIGQUIT is not pending!\n");
    }
    printf("after sleep, sig%d is coming\n", signum);
}

int main()
{
    struct sigaction act;
    bzero(&act,sizeof(act));
    act.sa_flags = SA_SIGINFO; //新类型
    act.sa_sigaction = sigfunc;
    //屏蔽3号信号
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGQUIT);
    act.sa_mask = sigset;

    int ret = sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret,-1,"sigaction");

    while(1);
    return 0;
}
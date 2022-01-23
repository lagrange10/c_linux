#include <func.h>

int main(int argc,char* argv[])
{
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        printf("i am child, pid=%d, ppid=%d\n",getpid(),getppid());
        exit(3);
    }
    else
    {
        printf("i am parent, my child is %d, pid=%d, ppid=%d\n", pid, getpid(),getppid());
        int status;
        pid = wait(&status);
        //status隐含以下信息的二进制位组合，需要宏将对应的位取出
        //掌握WIFEXITED：子进程是否正常退出？
        //掌握WEXITSTATUS：子进程退出返回值是多少？
        if(WIFEXITED(status))
        {
            printf("child is %d, exit with %d\n", pid, WEXITSTATUS(status));
        }
        else
        {
            puts("child crash!\n");
        }
        
        // sleep(1);
    }
    return 0;
}
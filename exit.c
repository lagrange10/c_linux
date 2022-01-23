#include <func.h>

//exit从子函数让进程结束
int print()
{
    printf("I am print func\n");
    //echo $?看最后一个执行程序的返回值
    // exit(-1);
    return 0;
}
int main()
{
    print();
    printf("I am main func\n");
    return 0;
}
#include <func.h>

void cleanup(void *p)
{
    printf("i am cleanup%ld\n",(long)p);
}

void * thread_func(void *p)
{
    pthread_cleanup_push(cleanup,(void *)1);
    pthread_cleanup_push(cleanup,(void *)2);
    //cancel点，异步
    //3种情况下检查cleanup栈：exit,cancel,pop(1)
    printf("i am child thread\n");
    sleep(1);
    printf("i am child thread: i am not canceled\n");
    pthread_cleanup_pop(1);  //0：pop了但什么都不做  1：pop并执行清理函数
    pthread_cleanup_pop(1);
}

int main()
{
    int ret;
    //创建子线程
    pthread_t pthid;
    ret = pthread_create(&pthid,NULL,thread_func,NULL);
    THREAD_ERR_CHECK(ret,"pthread_create");
    
    ret = pthread_cancel(pthid);
    THREAD_ERR_CHECK(ret,"pthread_cancel");
    pthread_join(pthid,NULL);

    return 0;
}
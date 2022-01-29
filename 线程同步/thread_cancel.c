#include <func.h>

void * thread_func(void *p)
{
    printf("i am child thread\n");
    sleep(1);
    printf("i am child thread: i am not canceled\n");
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
    printf("i am main thread\n");

    return 0;
}
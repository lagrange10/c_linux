#include <func.h>

void * thread_func(void *p)
{
    printf("i am child thread, get %d \n",*(int *)p);
    pthread_exit((void *)2);
}

int main()
{
    int ret ,i=1;
    //创建子线程
    pthread_t pthid;
    ret = pthread_create(&pthid,NULL,thread_func,&i);
    THREAD_ERR_CHECK(ret,"pthread_create");
    
    //等待子线程
    pthread_join(pthid,(void **)&ret);
    printf("i am main thread, child return %d \n", ret);

    return 0;
}
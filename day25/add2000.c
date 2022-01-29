#include <func.h>
#define N 20000000

typedef struct
{
    int num;
    pthread_mutex_t mutex;
} data_t;

void * thread_func(void *p)
{
    data_t *d = (data_t *)p;
    for(int i=0;i<N;i++)
    {
        pthread_mutex_lock(&d->mutex);
        d->num++;
        pthread_mutex_unlock(&d->mutex);
    }
    pthread_exit(NULL);
}

int main()
{
    int ret;
    //初始化锁mutex
    pthread_mutex_t mutex;
    ret = pthread_mutex_init(&mutex,NULL);
    THREAD_ERR_CHECK(ret,"pthread_mutex_init");
    //初始化结构体数据d
    data_t d;
    d.num = 0;
    d.mutex = mutex;
    //创建子线程
    pthread_t pthid;
    ret = pthread_create(&pthid,NULL,thread_func,&d);
    THREAD_ERR_CHECK(ret,"pthread_create");
    //main 线程加2千万
    for(int i=0;i<N;i++)
    {
        pthread_mutex_lock(&d.mutex);
        d.num++;
        pthread_mutex_unlock(&d.mutex);
    }
    //等待子线程
    pthread_join(pthid,NULL);
    printf("d.num = %d \n", d.num);
    //销毁锁mutex
    ret = pthread_mutex_destroy(&d.mutex);
    THREAD_ERR_CHECK(ret,"pthread_mutex_destroy");

    return 0;
}
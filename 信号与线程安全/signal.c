#include <func.h>

typedef struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}data_t;

void * sub_func(void *p)
{
    data_t *d = (data_t *) p;
    pthread_mutex_lock(&d->mutex);
    pthread_cond_wait(&d->cond, &d->mutex);
    printf("i am subthread, i have been wakeup!\n");
    pthread_mutex_unlock(&d->mutex);
}

int main()
{
    int ret;
    
    data_t d;
    ret = pthread_mutex_init(&d.mutex,NULL);
    THREAD_ERR_CHECK(ret,"pthread_mutex_init");
    ret = pthread_cond_init(&d.cond, NULL);
    THREAD_ERR_CHECK(ret,"pthread_cond_init");

    pthread_t pthid;
    ret = pthread_create(&pthid, NULL, sub_func, (void*)&d);
    THREAD_ERR_CHECK(ret,"pthread_create");

    sleep(1);
    puts("i am main thread, now start signal!");
    ret = pthread_cond_signal(&d.cond);
    THREAD_ERR_CHECK(ret,"pthread_cond_signal");
    pthread_join(pthid,NULL); //等待子线程，否则主线程执行完毕
    puts("i am main thread, signal success!");

    return 0;
}
#include <func.h>

typedef struct 
{
    int num;
    pthread_mutex_t mutex;
}data_t;

void * sale_tickets1(void *p)
{
    data_t *d = (data_t *)p;
    while(1)
    {
        pthread_mutex_lock(&d->mutex);
        if (d->num > 0)
        {
            printf("i am sale_tickets1, start sale\n");
            d->num--;
            printf("i am sale_tickets1, end sale, remain %d\n", d->num);
            pthread_mutex_unlock(&d->mutex);
        } else {
            pthread_mutex_unlock(&d->mutex);
            break;
        }
    }
}

void * sale_tickets2(void *p)
{
    data_t *d = (data_t *)p;
    while(1)
    {
        pthread_mutex_lock(&d->mutex);
        if (d->num > 0)
        {
            printf("i am sale_tickets2, start sale\n");
            d->num--;
            printf("i am sale_tickets2, end sale, remain %d\n", d->num);
            pthread_mutex_unlock(&d->mutex);
        } else {
            pthread_mutex_unlock(&d->mutex);
            break;
        }
    }
}

int main()
{
    int ret;
    data_t d;
    d.num = 20;
    pthread_mutex_init(&d.mutex,NULL);
    //创建子线程
    pthread_t pthid[2];
    ret = pthread_create(&pthid[0],NULL,sale_tickets1,(void *)&d);
    THREAD_ERR_CHECK(ret,"pthread_create1");
    ret = pthread_create(&pthid[1],NULL,sale_tickets2,(void *)&d);
    THREAD_ERR_CHECK(ret,"pthread_create2");
    for(int i=0;i<2;i++)
    {
        pthread_join(pthid[i],NULL);
    }
    printf("i am main thread, remain %d tickets\n", d.num);

    return 0;
}
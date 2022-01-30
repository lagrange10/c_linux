#include <func.h>

#define N 10000000
typedef struct{
    int tickets;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}data_t;
//窗口1买票
void* sale_windows1(void *p)
{
    int i;
    data_t* d=(data_t*)p;
    while(1)
    {
        pthread_mutex_lock(&d->mutex);
        if(d->tickets>0)//一定要先加锁后去读
        {
            printf("windows1 start sale %d\n",d->tickets);
            d->tickets--;
            // 售票到0张的if判断在锁里面还是外面？
            // 如果在外面可能当要执行if判断时发生调度，sale_windows2此时拿到锁，以至于售出负票
            if (0 == d->tickets)
            {
                pthread_cond_signal(&d->cond);
            }
            pthread_mutex_unlock(&d->mutex);
            printf("windows1 finish sale %d\n",d->tickets);
        }else{
            //if判断在这里？这会导致这个线程break从而结束，导致只有一个买票线程
            pthread_mutex_unlock(&d->mutex);
            break;
        }
        sleep(1);
    }
}
//窗口2买票
void* sale_windows2(void *p)
{
    int i;
    data_t* d=(data_t*)p;
    while(1)
    {
        pthread_mutex_lock(&d->mutex);
        if(d->tickets>0)
        {
            printf("windows2 start sale %d\n",d->tickets);
            d->tickets--;
            if (0 == d->tickets)
            {
                pthread_cond_signal(&d->cond);
            }
            pthread_mutex_unlock(&d->mutex);
            printf("windows2 finish sale %d\n",d->tickets);
        }else{
            pthread_mutex_unlock(&d->mutex);
            break;
        }
        sleep(1);
    }
}

void *set_tickets(void * p)
{
    data_t *d = (data_t *) p;
    pthread_mutex_lock(&d->mutex);
    pthread_cond_wait(&d->cond, &d->mutex);
    // wait前半部：排队，解锁，睡觉 || wait后半部：醒来，加锁 
    printf("-----------------------------------------------\n");
    printf("i am set_tickets thread, set tickets 10\n");
    d->tickets = 10;
    pthread_mutex_unlock(&d->mutex);
}

//两个子线程卖票，加锁的位置
int main()
{
    int ret;
    data_t d;
    d.tickets=20;
    //初始化锁
    ret = pthread_mutex_init(&d.mutex,NULL);//初始化锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_init");
    //初始化条件变量
    ret = pthread_cond_init(&d.cond,NULL);//初始化条件变量
    THREAD_ERR_CHECK(ret,"pthread_cond_init");
    pthread_t pthid[3];
    // 让set_ticket先睡觉，故先创建pthid[2]，然后由将车票卖到0张的线程唤醒set_tickets。
    ret=pthread_create(&pthid[2],NULL,set_tickets,(void*)&d);
    THREAD_ERR_CHECK(ret,"pthread_create2");
    ret=pthread_create(&pthid[0],NULL,sale_windows1,(void*)&d);
    THREAD_ERR_CHECK(ret,"pthread_create0");
    ret=pthread_create(&pthid[1],NULL,sale_windows2,(void*)&d);
    THREAD_ERR_CHECK(ret,"pthread_create1");
    for(int i=0;i<3;i++)
    {
        pthread_join(pthid[i],NULL);//等待子线程
    }
    printf("main thread tickets=%d\n",d.tickets);
    ret=pthread_mutex_destroy(&d.mutex);//销毁锁
    THREAD_ERR_CHECK(ret,"pthread_mutex_destroy");
    return 0;
}
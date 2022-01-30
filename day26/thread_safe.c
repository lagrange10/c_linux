#include <func.h>

typedef struct{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}data_t;

void * sub_func(void *p)
{
    time_t now;
    time(&now);
    char *q = ctime(&now);
    printf("now is %s\n", q);
    sleep(3);
    printf("now is %s\n", q);
}

int main()
{
    int ret;
    
    pthread_t pthid;
    ret = pthread_create(&pthid, NULL, sub_func, NULL);
    THREAD_ERR_CHECK(ret,"pthread_create");
    sleep(1);

    time_t now;
    time(&now);
    char *p = ctime(&now);
    printf("i am main thread \nnow is %s\n", p);

    pthread_join(pthid,NULL);

    return 0;
}
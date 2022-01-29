#include <func.h>

void* subThread(void *p)
{
    int *q = (int *)p;
    *q = 5;
    printf("i am subthread, get %d\n", *q);
    pthread_exit(NULL);
}

int main()
{
    pthread_t pthid;
    int *p = (int *)malloc(4);
    *p=10;
    printf("i am mainthread, before %d\n", *p);
    int ret = pthread_create(&pthid, NULL, subThread, p);
    THREAD_ERR_CHECK(ret,"pthread_create");
    sleep(1);
    printf("i am mainthread, after %d\n", *p);
    return 0;
}
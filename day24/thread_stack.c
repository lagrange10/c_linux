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
    int x=10;
    printf("i am mainthread, before %d\n", x);
    int ret = pthread_create(&pthid, NULL, subThread, &x);
    THREAD_ERR_CHECK(ret,"pthread_create");
    sleep(1);
    printf("i am mainthread, after %d\n", x);
    return 0;
}
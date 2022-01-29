#include <func.h>
#define N 20000000

typedef struct 
{
    int num;
    pthread_mutex_t mutex;
}data_t;



int main()
{
    //共享内存与连接
    int shmid;
    shmid = shmget(1000,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmid");
    data_t *p = shmat(shmid,NULL,0);
    ERROR_CHECK(shmid,(data_t *)-1,"shmid");
    //定义并设置锁属性
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_setpshared(&mutexattr,PTHREAD_PROCESS_SHARED);
    //初始化结构指针p
    p->num = 0;
    pthread_mutex_init(&p->mutex,&mutexattr);
    int i;
    if (!fork())
    {
        for (i=0;i<N;i++)
        {
            pthread_mutex_lock(&p->mutex);
            p->num++;
            pthread_mutex_unlock(&p->mutex);
        }
        return 0;
    }
    else
    {
        for (i=0;i<N;i++)
        {
            pthread_mutex_lock(&p->mutex);
            p->num++;
            pthread_mutex_unlock(&p->mutex);
        }
        wait(NULL);
        printf("p->num=%d\n", p->num);
        shmctl(shmid,IPC_RMID,NULL);
        return 0;
    }
    
}
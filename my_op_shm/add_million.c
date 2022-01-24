#include <func.h>
#define N 10000000

int main()
{
    int shmid;
    shmid = shmget(1000,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid, 0, "shmid");
    int *p = (int *) shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char *)-1, "shmat");
    if(fork())
    {
        for(int i=0;i<N;i++) p[0]++;
        shmctl(shmid,IPC_RMID,NULL);
        printf("%d\n",p[0]);
        return 0;
    } 
    else
    {
        for(int i=0;i<N;i++) p[0]++;
        return 0;
    }
}
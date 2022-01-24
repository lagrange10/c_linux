#include <func.h>

int main()
{
    int shmid;
    shmid = shmget(1000,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid, 0, "shmid");
    char *p = (char *) shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char *)-1, "shmat");
    if(fork())
    {
        wait(NULL);
        printf("i am parent, %s\n",p);
        shmctl(shmid,IPC_RMID,NULL);
        return 0;
    } 
    else
    {
        strcpy(p,"How are you?");
        return 0;
    }
}
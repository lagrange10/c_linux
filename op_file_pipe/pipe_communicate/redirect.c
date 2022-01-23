#include <func.h>

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2);
    int fd,fd1;
    fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    printf("\n");
    close(STDOUT_FILENO);
    fd1 = dup(fd);
    printf("这是重定向到log1的文件\n");
    close(fd);
    return 0;
}

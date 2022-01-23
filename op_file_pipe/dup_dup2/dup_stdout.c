#include <func.h>

int main(int argc,char* argv[])
{
	ARGS_CHECK(argc,2);
    int fd;
    fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    printf("\n");
    close(STDOUT_FILENO);  //刷新stdout
    int fd1 = dup(fd);    //fd的复制，占据最小的空闲整数1即stdout
    printf("hello world!\n");  //相当于write(1,"hello world!\n",strlen("hello world!\n"));
    close(fd);
    
    return 0;

}
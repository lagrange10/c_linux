#include <func.h>

int main(int argc,char* argv[])
{
	ARGS_CHECK(argc,2);
    int fd;
    fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    printf("fd=%d\n", fd);
    int fd1 = dup(fd);    //fd的复制，open里面的是赋值
    printf("fd1=%d\n", fd1);
    char buf[128]={0};
    int ret = read(fd,buf,5);
    ERROR_CHECK(ret,-1,"read");
    printf("buf=%s\n", buf);
    printf("-------------\n");  //fd和fd1所指的文件对象（一个结构体）是同一份，里面有光标位置。
    memset(buf,0,sizeof(buf));
    read(fd1,buf,5);
    printf("buf=%s\n", buf);
    close(fd1);
    
    return 0;

}
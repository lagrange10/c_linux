#include <func.h>

int main(int argc,char* argv[])
{
	ARGS_CHECK(argc,2);
    int fd;
    fd = open(argv[1],O_RDONLY);
    printf("i am read process!\n");
    char buf[128]={0};
    int ret = read(fd,buf,sizeof(buf));
    printf("ret=%d, reader=%s", ret, buf);
    close(fd);
    return 0;
}
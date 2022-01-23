#include <func.h>

int main(int argc,char* argv[])
{
	ARGS_CHECK(argc,2);
    int fd;
    fd = open(argv[1],O_WRONLY);
    printf("i am write process!\n");
    // write(fd,"hello",5);
    close(fd);
    sleep(10);
    return 0;
}

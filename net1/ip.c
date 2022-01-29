#include <func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    struct in_addr addr;
    int ret = inet_aton(argv[1],&addr);
    if (0 == ret)
    {
        printf("inet_aton failed! \n");
        return -1;
    }
    printf("internet address is %x\n", addr.s_addr);
    printf("点分十进制地址为 %s\n", inet_ntoa(addr));
    printf("internet address is %x\n", inet_addr(argv[1]));
    return 0;
}
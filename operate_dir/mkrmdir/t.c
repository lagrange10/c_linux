#include <stdio.h>
#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2);
    int ret;
    ret=mkdir(argv[1],777);
    ERROR_CHECK(ret,"mkdir");
    return 0;
}


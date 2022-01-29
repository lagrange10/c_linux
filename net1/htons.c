#include <func.h>

int main()
{
    short port = 0x1234, nport, hport;
    nport = htons(port);
    printf("nport = %x \n", nport);  //0x3412
    hport = ntohs(nport);
    printf("hport = %x \n", hport);

    return 0;
}
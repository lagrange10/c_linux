#include <stdio.h>
#include <unistd.h>

int main()
{
	char buf[512];
	getcwd(buf,sizeof(buf));
	puts(buf);
	return 0;
}

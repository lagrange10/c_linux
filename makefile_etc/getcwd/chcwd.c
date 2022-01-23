#include <stdio.h>
#include <unistd.h>

int main()
{
	char buf[512];
	getcwd(buf,sizeof(buf));
	puts(buf);
	chdir("../");
	puts(getcwd(NULL,0));
	return 0;
}

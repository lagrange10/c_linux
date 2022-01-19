#include <func.h>

int main(int argc,char* argv[])
{
	ARGS_CHECK(argc,2);
    DIR *dir=opendir(argv[1]);
    struct dirent *p;
    off_t pos;
    while(p=readdir(dir))
    {
        printf("inode=%ld\ttype=%d\toffset=%ld\tlen=%d\tname=%s\n", p->d_ino, p->d_type,\
        p->d_off,p->d_reclen,p->d_name);
        if(!strcmp(p->d_name,"mkdir.c"))
            pos=telldir(dir);
    }
    seekdir(dir,pos);
    p=readdir(dir);
    printf("inode=%ld\ttype=%d\toffset=%ld\tlen=%d\tname=%s\n", p->d_ino, p->d_type,\
        p->d_off,p->d_reclen,p->d_name);
    rewinddir(dir);
    p=readdir(dir);
    printf("inode=%ld\ttype=%d\toffset=%ld\tlen=%d\tname=%s\n", p->d_ino, p->d_type,\
        p->d_off,p->d_reclen,p->d_name);
    closedir(dir);
    return 0;
}
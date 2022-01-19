#include <func.h>

char * oct_to_humanrd(int i, char *c)
{
    if(i>=8 && i<=0) printf("oct_to_humanrd input error\n");
    c[2] = (i%2)==1?'x':'-'; i /= 2;
    c[1] = (i%2)==1?'w':'-'; i /= 2;
    c[0] = (i%2)==1?'r':'-'; i /= 2;
    return c;
}

int main(int argc,char* argv[])
{
	ARGS_CHECK(argc,2);
    DIR *dir=opendir(argv[1]);
    struct dirent *p;
    struct stat buf;
    char bufTime[32],dir_cat[1024];
    long long mode,type,user,group,others; int ret;
    char f_type;
    // c1 c2 c3 is string of a human-readable mode
    char c1[4]={0};
    char c2[4]={0};
    char c3[4]={0};
    while(p=readdir(dir))
    {
        sprintf(dir_cat,"%s%s%s", argv[1],"/",p->d_name);
        ret = stat(dir_cat,&buf);
        ERROR_CHECK(ret,-1,"stat");
        strcpy(bufTime, ctime(&buf.st_mtime));
        bufTime[strlen(bufTime)-1]=0; //remove \n
        //get type of file
        mode=buf.st_mode;
        type=mode/(int) pow(16,3);
        f_type=type==4?'d':'-';
        //get mode of user
        mode = mode % (int)pow(16,3);
        /* now mode have 9 bits, and every 3 bits represents user, group
        and others respectively! */
        user = mode / (int)pow(2,6);
        group = (mode / (int)pow(2,3)) % (int)pow(2,3);
        others = mode % (int)pow(2,3);
        //printf("$mode=%d %d %d$",user,group,others);
        printf("%c%s%s%s %ld %s %s %-6ld %s %s\n",f_type, oct_to_humanrd(user,c1), \
        oct_to_humanrd(group,c2), oct_to_humanrd(others,c3), \
        buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,\
         buf.st_size,bufTime+4, p->d_name);
        /* do not use the same string (eg:char *c) to oct_to_humanrd or it will be overwrite
        by a LIFO order of oct_to_humanrd */
    }
    return 0;
}
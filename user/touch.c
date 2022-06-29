#include "lib.h"


void touch(char* filename)
{
    struct Stat st;
    int r;
    int n;
    int fd;
    struct File f;
    int flag=0;
    // writef("filename: %s-\n",filename);
    r=stat("/",&st);
    if(r<0){
        user_panic("stat /: %e",r);
    }
    // writef("ok1");
    if ((fd = open("/", O_RDONLY)) < 0)
		user_panic("open /: %e", fd);
    // writef("ok2");
	while ((n = readn(fd, &f, sizeof f)) == sizeof f){
        // writef("%s ",f.f_name);
        if(f.f_name[0])
            if (strcmp(f.f_name,filename)==0){
                flag=1;
                break;
            }
    }
		
    // writef("ok%d",flag);
    user_create(filename,2);
    return;
    if(flag==1){
        writef("file already exist");
    }else{
        int fd=open(filename,O_CREAT);
        close(fd);
    }
}

void
usage(void)
{
	writef("touch: [filename...]\n");
	exit();
}

void
umain(int argc, char **argv)
{
	int i;
    if(argc<=1){
        usage();
    }else{
        for(i=1;i<argc;i++){
            touch(argv[i]);
        }
    }
}



#include "lib.h"
char fname[MAXNAMELEN];


void touch(char* filename)
{
    fname[0]='u';
    fname[1]='s';
    fname[2]='r';
    fname[3]='/';
    if(filename[0]=='/'){
        strcpy(fname+4,filename+1);
    }else{
        strcpy(fname+4,filename);    
    }
    user_create(fname,2);
    return;

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



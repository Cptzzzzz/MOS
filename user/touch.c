#include "lib.h"
char fname[MAXNAMELEN];


void touch(char* filename)
{
    fname[0]='h';
    fname[1]='o';
    fname[2]='m';
    fname[3]='e';
    fname[4]='/';
    if(filename[0]=='/'){
        strcpy(fname+5,filename+1);
    }else{
        strcpy(fname+5,filename);    
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



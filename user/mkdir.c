#include "lib.h"
char fname[MAXNAMELEN];


void mkdir(char* filename)
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
    user_create(fname,1);
    return;
}

void
usage(void)
{
	writef("mkdir: [foldername...]\n");
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
            mkdir(argv[i]);
        }
    }
}


// void
// umain(int argvn, char** argv) {
//     int off = 0;
//     int r = 0;
//     if (argvn == 3 && strcmp(argv[1], "-p")==0) {
//         off = 1;
//     }
//     if ((r = user_create(argv[1+off], 1))<0) {
//         writef("create path %s failed!\n", argv[1+off]);
//     }
// }

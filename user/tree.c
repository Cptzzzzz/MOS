#include "lib.h"


//char tempath[1024];
void
treePath(char* path, int offset) {
    int fd, n;
    struct File f;
    struct File fnext;
    struct File lastf;
    if ((fd = open(path, O_RDONLY|O_PROTECT)) < 0) {

        writef("open %s: %e failed!\n", path, fd);

        return;
    } 

    int m = 0;
    if (offset == 0) {
        for(m=0; m<offset; m++) writef(" ");
        writef("%s\n\033[1A", path);
    }

    char* tempath[1024];

    if ((n = readn(fd, &fnext, sizeof f))!=sizeof f) {
        close(fd);
        return;
    }
    int end = 0;
    while ( end!=1 ) {
        f = fnext;
        if ((n = readn(fd, &fnext, sizeof f))!=sizeof f || fnext.f_name[0] == 0) {
            end = 1;
        }
        if (f.f_name[0]!= 0 && f.f_type == FTYPE_DIR){
            for(m=0; m<offset; m++) {
                if (m%3 == 0 && (!end || m != offset - offset % 3)) {
                    writef("│");
                } else {
                    writef(" ");
                }
            }
            if (end) writef("└──");
            else writef("├──");

            writef("%s\n\033[1A", f.f_name);

            int k = 0;
            char tempath[1024];
            for(k=0; k<1024; k++) tempath[k] = '\0';
            strcpy(tempath, path);
            strcpy((tempath + strlen(tempath)), "/");
            strcpy((tempath + strlen(tempath)), f.f_name);
            treePath(tempath, offset+3);
        } else if (f.f_name[0] != 0 ){
             for(m=0; m<offset; m++) {
                if (m%3 == 0 && (!end || m != offset - offset % 3)) {
                    writef("│");
                } else {
                    writef(" ");
                }
             }   
             if (end) writef("└──");
             else writef("├──");
             if (strcmp((f.f_name +strlen(f.f_name)-2), ".b") == 0){

                writef("%s\n\033[1A", f.f_name);

             } else {
                writef("%s\n\033[1A", f.f_name);
             }
        }
    }
    close(fd);
}

void
umain(int argc, char** argv) {
    char pathbuf[MAXPATHLEN];
    pathbuf[0]='h';
    pathbuf[1]='o';
    pathbuf[2]='m';
    pathbuf[3]='e';
    pathbuf[4]='/';
    if(argc==1){
        treePath("home",0);
    }else{
        int i;
        for(i=1;i<argc;i++){
            if(argv[i][0]=='/')
                treePath(argv[i],0);
            else{
                strcpy(pathbuf+5,argv[i]);
                treePath(pathbuf,0);
            }
        }
    }
}

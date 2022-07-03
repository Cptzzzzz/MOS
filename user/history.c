#include "lib.h"

char buf[8192];

void
history(int f, char *s)
{
	long n;
	int r;

	while((n=read(f, buf, (long)sizeof buf))>0){
		writef("%s",buf);
    }
	if(n < 0)
		user_panic("error reading %s: %e", s, n);
}

void
umain(int argc, char **argv)
{
	int f, i;
    f = open("etc/history", O_RDONLY|O_PROTECT);
    if(f < 0)
        user_panic("can't open %s: %e", argv[i], f);
    else{
        history(f,"history");
        close(f);
    }

}


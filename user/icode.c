#include "lib.h"

void
umain(void)
{
	int fd, n, r;
	char buf[512+1];

	writef("icode: open /motd\n");
	if ((fd = open("bin/motd", O_RDONLY|O_PROTECT)) < 0)
		user_panic("icode: open /motd: %e", fd);

	writef("icode: read /motd\n");
	while ((n = read(fd, buf, sizeof buf-1)) > 0){
		buf[n] = 0;
		writef("%s\n",buf);
	}

	writef("icode: close /motd\n");
//	close(fd);
	user_create("etc/history",2);
	user_create("etc/variables",2);
	writef("icode: spawn /init\n");
	if ((r = spawnl("init.b", "init", "initarg1", "initarg2", (char*)0)) < 0)
		user_panic("icode: spawn /init: %e", r);

	writef("icode: exiting\n");
}

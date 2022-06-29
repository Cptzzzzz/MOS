#include "lib.h"

int flag[256];

void lsdir(char*, char*);
void ls1(char*, u_int, u_int, char*);

void
ls(char *path, char *prefix)
{
	int r;
	struct Stat st;
	// writef("path:-%d-\n",path);
	if ((r=stat(path, &st)) < 0)
		user_panic("stat %s: %e", path, r);
	if (st.st_isdir && !flag['d']){
		// writef("is dir\n");
		lsdir(path, prefix);
	}else{
		writef("ls1\n");
		ls1(0, st.st_isdir, st.st_size, path);
	}
		
}

void
lsdir(char *path, char *prefix)
{
	int fd, n;
	struct File f;

	if ((fd = open(path, O_RDONLY)) < 0)
		user_panic("open %s: %e", path, fd);
	while ((n = readn(fd, &f, sizeof f)) == sizeof f)
		if (f.f_name[0])
			ls1(prefix, f.f_type==FTYPE_DIR, f.f_size, f.f_name);
	if (n > 0)
		user_panic("short read in directory %s", path);
	if (n < 0)
		user_panic("error reading directory %s: %e", path, n);
}

void
ls1(char *prefix, u_int isdir, u_int size, char *name)
{
	char *sep;

	if(flag['l'])
		writef( "%11d %c ", size, isdir ? 'd' : '-');
	if(prefix) {
		if (prefix[0] && prefix[strlen(prefix)-1] != '/')
			sep = "/";
		else
			sep = "";
		writef( "%s%s", prefix, sep);
	}
	writef( "%s", name);
	if(flag['F'] && isdir)
		fwritef(1, "/");
	writef( " ");
}

void
usage(void)
{
	writef("usage: ls [-dFl] [file...]\n");
	exit();
}

void
umain(int argc, char **argv)
{
	int i;

	ARGBEGIN{
	default:
		usage();
	case 'd':
	case 'F':
	case 'l':
		flag[(u_char)ARGC()]++;
		break;
	}ARGEND

	if (argc == 0)
		ls("/", "");
	else {
		for (i=0; i<argc; i++)
			ls(argv[i], argv[i]);
	}
}



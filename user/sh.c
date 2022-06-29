#include "lib.h"
#include <args.h>

int debug_ = 0;

//
// get the next token from string s
// set *p1 to the beginning of the token and
// *p2 just past the token.
// return:
//	0 for end-of-string
//	> for >
//	| for |
//	w for a word
//
// eventually (once we parse the space where the nul will go),
// words get nul-terminated.
#define WHITESPACE " \t\r\n"
#define SYMBOLS "<|>&;()"
char history_buf[4096];
char* history_ptr=0;
char* history_maxl=0;
char temp_buf[1024];
int
_gettoken(char *s, char **p1, char **p2)
{
	int t;

	if (s == 0) {
		//if (debug_ > 1) writef("GETTOKEN NULL\n");
		return 0;
	}

	//if (debug_ > 1) writef("GETTOKEN: %s\n", s);

	*p1 = 0;
	*p2 = 0;

	while(strchr(WHITESPACE, *s))
		*s++ = 0;
	if(*s == 0) {
	//	if (debug_ > 1) writef("EOL\n");
		return 0;
	}
	if(strchr(SYMBOLS, *s)){
		t = *s;
		*p1 = s;
		*s++ = 0;
		*p2 = s;
//		if (debug_ > 1) writef("TOK %c\n", t);
		return t;
	}
	if(*s=='\"'){
		s++;
		*p1=s;
		while(*s!='\"'){
			s++;
		}
		*s=' ';
		*p2=s;
		return 'w';
	}
	*p1 = s;
	while(*s && !strchr(WHITESPACE SYMBOLS, *s))
		s++;
	*p2 = s;
	if (debug_ > 1) {
		t = **p2;
		**p2 = 0;
//		writef("WORD: %s\n", *p1);
		**p2 = t;
	}
	return 'w';
}

int
gettoken(char *s, char **p1)
{
	static int c, nc;
	static char *np1, *np2;

	if (s) {//第一次调用gettoken
		nc = _gettoken(s, &np1, &np2);
		return 0;
	}
	//后续调用gettoken
	// writef("c=%c-\n",nc);
	c = nc;
	*p1 = np1;
	nc = _gettoken(np2, &np1, &np2);
	return c;
}

#define MAXARGS 16
int backstage=0;
int runnow=0;
void
runcmd(char *s)
{
	char *argv[MAXARGS], *t;
	int argc, c, i, r, p[2], fd, rightpipe;
	int fdnum,cmdlength;
	// cmdlength=strlen(s);
	// if(s[cmdlength-1]=='&'){
	// 	isrun=1;
	// 	s[cmdlength-1]='\0';
	// }
	// writef("in runcmd\n");
	rightpipe = 0;
	gettoken(s, 0);
	runnow=0;
again:
	argc = 0;
	for(;;){
		c = gettoken(0, &t);
		switch(c){
		case 0:
			goto runit;
		case ';':
			runnow=1;
			goto runit;
		case '&':
			backstage=1;
			break;
		case 'w':
			if(argc == MAXARGS){
				writef("too many arguments\n");
				exit();
			}
			argv[argc++] = t;
			break;
		case '<':
			if(gettoken(0, &t) != 'w'){
				writef("syntax error: < not followed by word\n");
				exit();
			}
			// Your code here -- open t for reading,
			fd=open(t,O_RDONLY);
			if(r<0){
				writef("case '<' : open t failed\n");
                exit();
			}
			dup(fd,0);
			close(fd);
			// dup it onto fd 0, and then close the fd you got.
			// user_panic("< redirection not implemented");
			break;
		case '>':
			// Your code here -- open t for writing,
			// dup it onto fd 1, and then close the fd you got.
			if(gettoken(0,&t)!='w'){
				writef("syntax error: < not followed by word\n");
				exit();
			}
			fd = open(t, O_WRONLY|O_CREAT);
            if (r<0) {
                writef("case '>' : open t failed\n");
                exit();
            }
			dup(fd,1);
			close(fd);
			// user_panic("> redirection not implemented");
			break;
		case '|':
			// Your code here.
			// 	First, allocate a pipe.
			//	Then fork.
			//	the child runs the right side of the pipe:
			//		dup the read end of the pipe onto 0
			//		close the read end of the pipe
			//		close the write end of the pipe
			//		goto again, to parse the rest of the command line
			//	the parent runs the left side of the pipe:
			//		dup the write end of the pipe onto 1
			//		close the write end of the pipe
			//		close the read end of the pipe
			//		set "rightpipe" to the child envid
			//		goto runit, to execute this piece of the pipeline
			//			and then wait for the right side to finish
			r=pipe(p);
			if (r < 0) {
                user_panic("BUG: in runcmd: pipe: %e\n", r);
            }
			r=fork();
			if (r<0) {
                writef("| not implemented (fork)\n");
                exit();
            }
			if (r==0) { //this is child.
                dup(p[0], 0); // p[0] becomes stdin.
                close(p[0]);
                close(p[1]);
                goto again;
            }
			if (r > 0) { //this is parent.
                dup(p[1], 1);// p[1] becomes stdout.
                close(p[1]);
                close(p[0]);
                rightpipe = r;
                goto runit;
            }
			// user_panic("| not implemented");
			break;
		}
	}

runit:
	if(argc == 0) {
		if (debug_) writef("EMPTY COMMAND\n");
		return;
	}
	argv[argc] = 0;
	if (0) {
		writef("[%08x] SPAWN:", env->env_id);
		for (i=0; argv[i]; i++)
			writef(" %s", argv[i]);
		writef("\n");
	}
	// writef("length: %d\n",argc);
	if ((r = spawn(argv[0], argv)) < 0)
		writef("spawn %s: %e\n", argv[0], r);
	close_all();
	// writef("isrun: %d\n",isrun);
	if (r >= 0) {
		if (debug_) writef("[%08x] WAIT %s %08x\n", env->env_id, argv[0], r);
		// if(isrun==0)
		if(backstage==0)
			wait(r);
		else
			backstage=0;
	}
	if (rightpipe) {
		if (debug_) writef("[%08x] WAIT right-pipe %08x\n", env->env_id, rightpipe);
		// if(isrun==0)
			wait(rightpipe);
	}
	if(runnow==1){
		runnow=0;
		goto again;
	}
	exit();
}


void debug_msg(int index,int length)
{
	writef("\033[1A");
	writef("---");
	writef("%d %d",index,length);
	writef("---");
	writef("\033[1B");
}
void flush(char *buf,int length,int index)
{
	// writef("\033[%dX",length-index);
	// int i;
	// for(i=0;i<index;i++){
	// 	writef("\b \b");
	// }
	// debug_msg(index,length);
	writef("\033[u");
	writef("\033[K");
	// writef("\033[0K");
	int i;
	for(i=0;i<length;i++)
	writef("%c",buf[i]);
	for(i=length-index;i>0;i--){
		writef("\033[1D");
	}
	writef("\033[s");
	// writef("\033[%dD",length-index);
}
void addchar(char* buf,int length,int index,char t)
{
	int i;
	for(i=length;i>index;i--){
		buf[i]=buf[i-1];
	}
	buf[index]=t;
	writef("\033[1@");
}
void delchar(char* buf,int length,int index)
{
	int i;
	if(index==0)return;
	for(i=index;i<length;i++){
		buf[i-1]=buf[i];
	}
	writef("\033[1D");
	writef("\033[1P");
}
void save_command(char * buf)
{
	// writef("saved: %d %s\n",strlen(buf),buf);
	int fd = open("history", O_WRONLY|O_CREAT|O_APPEND);
	write(fd,buf,strlen(buf));
	write(fd,"\n",1);
	close(fd);
}
void init_buf()
{
	history_buf[0]=0;
	temp_buf[0]=0;
	int fd=open("history",O_RDONLY|O_CREAT);
	int n=read(fd,history_buf,4096);
	close(fd);
	// writef("ok");
	// writef("-%d-\n",strlen(history_buf));
	if(n<0)
		user_panic("error reading history");
	if(history_buf[0]==0){
		history_maxl=0;
		history_ptr=0;
		return;
	}
	history_maxl=history_buf;
	while(*history_maxl!='\0'){
		if(*history_maxl=='\n'){
			*history_maxl='\0';
			// writef("ok");
		}
		history_maxl++;
	}
	history_maxl--;
	history_ptr=history_maxl+1;
	return;
}

int history_last(char *buf,int length)
{
	buf[length]=0;
	// writef("up");
	if(history_ptr==history_maxl){
		return 0;
	}
	if(history_ptr==history_buf){
		return 0;
	}
	if(history_ptr==history_maxl+1){
		strcpy(temp_buf,buf);
	}
	history_ptr-=2;
	while(*history_ptr!=0){
		history_ptr--;
	}
	history_ptr++;
	strcpy(buf,history_ptr);
	return 1;
}
int history_next(char *buf,int length)
{
	if(history_ptr==history_maxl){
		return 0;
	}
	if(history_ptr==history_maxl+1){
		return 0;
	}
	while(*history_ptr!='\0'){
		history_ptr++;
	}
	history_ptr++;
	if(history_ptr==history_maxl+1){
		strcpy(buf,temp_buf);
	}else{
		strcpy(buf,history_ptr);
	}
	return 1;
}
void
readline(char *buf, u_int n)
{
	int i, r;
	char t;
	int index=0;
	r = 0;
	init_buf();
	for(i=0;;){
		//0x9 tab
		//0x7f backspace
		//0x41-0x44  up down right left
		if((r = read(0,&t, 1)) != 1){
			if(r < 0)
				writef("read error: %e", r);
			exit();
		}
		
		if(t==0x9){
			//todo tab

		}else if(t==0x7f){
			//todo backspace
			if(index!=0){
				delchar(buf,i,index);
				i--;
				index--;
				continue;
			}
		}else if(t==0x1b){
			if ((r = read(0,&t, 1) != 1)) {
                fwritef(1, "wrong when press up!\n");
            }
			if ((r = read(0,&t, 1) != 1)) {
                fwritef(1, "wrong when press up!\n");
            }
			if(t==0x41){
				if(history_last(buf,i)){
					i=strlen(buf);
					index=i;
					writef("i:%d index:%d\n",i,index);
				}
			}else if(t==0x42){
				if(history_next(buf,i)){
					i=strlen(buf);
					index=i;
				}
			}else if(t==0x43){
				if(index<i){
					index++;
					// writef("\033[1C");
				}
				// writef("\033[1D");
				// continue;
			}else if(t==0x44){
				if(index>0){
					index--;
					// writef("\033[1D");
				}
				// writef("\033[1C");
				// continue;
			}
		}else if(t == '\r' || t == '\n'){
			buf[i] = 0;
			save_command(buf);
			return;
		}else{
			addchar(buf,i,index,t);
			i++;
			index++;
			if(i==1023){
				buf[i]=0;
				return;
			}
			continue;
		}
		flush(buf,i,index);
		writef("\033[u");
	}
	writef("line too long\n");
	while((r = read(0, buf, 1)) == 1 && buf[0] != '\n')
		;
	buf[0] = 0;
}	

char buf[1024];

void
usage(void)
{
	writef("usage: sh [-dix] [command-file]\n");
	exit();
}

void
umain(int argc, char **argv)
{
	int r, interactive, echocmds;
	interactive = '?';
	echocmds = 0;
	writef("\n:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	writef("::                                                         ::\n");
	writef("::              Super Shell  V0.0.0_1                      ::\n");
	writef("::                                                         ::\n");
	writef(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	ARGBEGIN{
	case 'd':
		debug_++;
		break;
	case 'i':
		interactive = 1;
		break;
	case 'x':
		echocmds = 1;
		break;
	default:
		usage();
	}ARGEND

	if(argc > 1)
		usage();
	if(argc == 1){
		close(0);
		if ((r = open(argv[1], O_RDONLY)) < 0)
			user_panic("open %s: %e", r);
		user_assert(r==0);
	}
	if(interactive == '?')
		interactive = iscons(0);
	for(;;){
		if (interactive)
			fwritef(1, "\n$ ");
		writef("\033[s");
		readline(buf, sizeof buf);
		// writef("length:%d\n",strlen(buf));
		// writef("read: %s\n",buf);

		if (buf[0] == '#')
			continue;
		if (echocmds)
			fwritef(1, "# %s\n", buf);
		if ((r = fork()) < 0)
			user_panic("fork: %e", r);
		// writef("ready fork %d \n",r);
		if (r == 0) {
			// writef("son %s\n",buf);
			runcmd(buf);
			exit();
			return;
		} else{
			// writef("father:%s\n",buf);
			wait(r);
		}
		
	}
}

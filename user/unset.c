#include "lib.h"

int envid;
char variable_buf[4096];

int get_detail(char **p,char **name,int *r,int* xx,int *pid,char** value)
{
	//!读取该行变量数据 *p改为下一行的起始位置
	//! 把间隔符设置成\0 *name指向variable_buf中name起始地址
	//! r x pid设置为对应的值
	//! *value指向variable_buf中value的位置
	//! 返回值为1表示当前读到了 否则没读到
	if(**p=='\0') return 0;
	char *x=*p;
	while(*x!=' '){
		x++;
	}
	*x='\0';
	*name=*p;
	x++;
	if(*x=='r'){
		*r=1;
	}else{
		*r=0;
	}
	x++;
	*x='\0';
	x++;
	if(*x=='x'){
		*xx=1;
	}else{
		*xx=0;
	}
	x++;
	*x='\0';
	x++;
	if(*x=='-'){
		*pid=-1;
		x++;
	}else{
		int res=0;
        // printf("%c",*x);
		while(isdigit(*x)){
			res=res*10+(*x-'0');
			x++;
		}
        // printf("%d\n",res);
		*pid=res;
	}
	// x++;
	if(*x=='\n'){
		*value=0;
	}else{
		*value=x;
	}
	while(*x!='\n'){
		x++;
	}
    *x='\0';
	x++;
	*p=x;
	return 1;
}
void remove_variable(char *stop)
{
	int fd=open("etc/variables",O_RDONLY|O_PROTECT);
	read(fd,variable_buf,4096);
	close(fd);
	// writef("%d\n",strlen(variable_buf));
	// writef("replace: %s\n",stop);
	*stop=0;
	// writef("replace1: %s %d\n",variable_buf,strlen(variable_buf));
	while(*stop!='\n')stop++;
	stop++;
	// writef("replace2: %s %d\n",stop,strlen(stop));
	fd=open("etc/variables",O_WRONLY|O_PROTECT);
	write(fd,variable_buf,strlen(variable_buf));
	write(fd,stop,strlen(stop));
	close(fd);
}
void del_variable(char *pname,int px)
{
	int fd=open("etc/variables",O_RDONLY|O_CREAT|O_PROTECT);
	read(fd,variable_buf,4096);
	close(fd);
	char *p=variable_buf;
	char *stop=0;
	int r,x,pid;
	char *name,*value;
	stop=p;
	int flag=0;
	while(get_detail(&p,&name,&r,&x,&pid,&value)){
		if(strcmp(name,pname)==0){
			if(x==px){
                if(x==0){
                    if(pid!=envid){
                        stop=p;
                        continue;
                    }
                }
				flag=1;
				break;
			}
		}
		stop=p;
	}
	if(flag){
		if(r){
			writef("can not remove readonly ");
			if(x)
				writef("global variable ");
			else
				writef("local variable ");
			writef("%s",name);
			return;
		}
		remove_variable(stop);
		writef("successfully remove ");
		if(x)
			writef("global variable ");
		else
			writef("local variable ");
		writef("%s",name);
	}else{
		writef("no ");
		if(px)
		writef("global variable named %s",pname);
		else
		writef("local variable named %s",pname);
	}
}
int atoi(char *buf)
{
    int res=0;
    int i=0;
    for(;buf[i]!='\0';i++){
        res=10*res+buf[i];
    }
    return res;
}
void umain(int argc,char **argv)
{
    // writef("unst");
    envid=atoi(argv[0]);
	char *name;
	int x=0;
	int i;
	int xp=0,namep=0;
	for(i=1;i<argc;i++){
		if(argv[i][0]=='-'){
			if(xp==1)continue;
			if(argv[i][1]=='x'){
				x=1;
			}
			xp=1;
		}else{
			if(namep==1)continue;
			name=argv[i];
			namep=1;
		}
	}
	if(namep){
        // writef("??\n");
		del_variable(name,x);
	}else{
        // writef("??\n");
		writef("usage: unset name [-x]");
	}
}
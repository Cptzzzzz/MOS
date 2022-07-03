#include "lib.h"


char variable_buf[4096];
int envid;
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
void print_all_variable()
{
	int fd=open("etc/variables",O_RDONLY|O_CREAT|O_PROTECT);
	read(fd,variable_buf,4096);
	close(fd);
	char *p=variable_buf;
	int r,x,pid;
	char *name,*value;
	if(*p=='\0'){
		writef("no variables now");
		return;
	}else{
		writef("name	r	x	pid	value\n");
	}
	while(get_detail(&p,&name,&r,&x,&pid,&value)){
		writef("%s\t",name);
		if(r==1) writef("1\t");
		else writef("0\t");
		if(x==1)writef("1\t");
		else writef("0\t");
		writef("%d\t",(pid==-1?0:pid));
		if(value==0){
			writef("UNDEFINED\n");
		}else{
			writef("%s\n",value);
		}
	}
	writef("declare variable list end");
}
void add_to_tail(char *name,int r,int x,char *value)
{
	// writef("add to tail\n");
	int fd=open("etc/variables",O_WRONLY|O_CREAT|O_APPEND|O_PROTECT);
	fwritef(fd,"%s ",name);
	fwritef(fd,"%c ",r==1?'r':'-');
	fwritef(fd,"%c ",x==1?'x':'-');
	if(x==1){
		fwritef(fd,"-");
	}else{
		fwritef(fd,"%d",envid);
	}
	if(value){
		fwritef(fd," %s",value);
	}
	fwritef(fd,"\n");
	close(fd);
	if(r)
	writef("added readonly ");
	else 
	writef("added ");
	if(x)
	writef("global ");
	else 
	writef("local ");
	writef("variable ");
	if(value)
	writef("%s=%s",name,value);
	else
	writef("%s",name);
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
//! 格式 name r x pid value
//! 0参数 输出全部变量 输出格式 name r x pid value(没有value则undefined)
//! 其他 定义变量
void add_variable(char *pname,int px,int pr,char *pvalue)
{
	int fd=open("etc/variables",O_RDONLY|O_CREAT|O_PROTECT);
	read(fd,variable_buf,4096);
	close(fd);
	char *p=variable_buf;
	char *stop=0;
	int r,x,pid;
	char *name,*value;
	int change=0;
	stop=p;
	while(get_detail(&p,&name,&r,&x,&pid,&value)){
		if(strcmp(name,pname)==0){
			if(x!=px){
				// continue;
			}else{
				if(x==0){
					if(envid==pid){
						change=1;
						break;
					}
				}else{
					change=1;
					break;
				}
			}
		}
		stop=p;
	}
	if(change==0){
		add_to_tail(pname,pr,px,pvalue);
		return;
	}
	if(r){
		if(x) writef("global ");
		else writef("local ");
		writef("variable %s can only be read",name);
		return;
	}
	remove_variable(stop);
	add_to_tail(pname,pr,px,pvalue);
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
void umain(int argc, char ** argv)
{
	// writef("argc:%d\n",argc);
    envid=atoi(argv[0]);
    // writef("envid:%d\n",envid);
    int i;
	int state=0;//0 输出变量
	int x=0,r=0;
	char *name;
	char *value=0;
	int go=0;
	int perm=0;
	int namep=0;
	int valp=0;
	for(i=1;i<argc;i++){
		if(argv[i][0]=='-'){
			if(perm==1){
				continue;
			}
			perm=1;
			if(argv[i][1]=='x'||argv[i][1]=='r'){
				if(argv[i][1]=='x'){
					x=1;
				}
				if(argv[i][1]=='r'){
					r=1;
				}
				if(argv[i][2]=='x'){
					x=1;
				}
				if(argv[i][2]=='r'){
					r=1;
				}
			}
		}else if(argv[i][0]=='='){
			if(valp==1){
				continue;
			}
			value=argv[i]+1;
			valp=1;
		}else{
			if(namep==1)continue;
			name=argv[i];
			namep=1;
		}
	}
	if(i==1){
		print_all_variable();
	}else{
		add_variable(name,x,r,value);
	}
}


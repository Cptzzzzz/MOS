#include "lib.h"


char variable_buf[4096];
int envid;
int get_detail(char **p,char **name,int *r,int* xx,int *pid,char** value)
{
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
		while(isdigit(*x)){
			res=res*10+(*x-'0');
			x++;
		}
		*pid=res;
	}
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
		fwritef(1,"no variables now");
		return;
	}else{
		fwritef(1,"name	r	x	pid	value\n");
	}
	while(get_detail(&p,&name,&r,&x,&pid,&value)){
		fwritef(1,"%s\t",name);
		if(r==1) fwritef(1,"1\t");
		else fwritef(1,"0\t");
		if(x==1)fwritef(1,"1\t");
		else fwritef(1,"0\t");
		fwritef(1,"%d\t",(pid==-1?0:pid));
		if(value==0){
			fwritef(1,"UNDEFINED\n");
		}else{
			fwritef(1,"%s\n",value);
		}
	}
	fwritef(1,"declare variable list end");
}
void add_to_tail(char *name,int r,int x,char *value)
{
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
	fwritef(1,"added readonly ");
	else 
	fwritef(1,"added ");
	if(x)
	fwritef(1,"global ");
	else 
	fwritef(1,"local ");
	fwritef(1,"variable ");
	if(value)
	fwritef(1,"%s=%s",name,value);
	else
	fwritef(1,"%s",name);
}
void remove_variable(char *stop)
{
	int fd=open("etc/variables",O_RDONLY|O_PROTECT);
	read(fd,variable_buf,4096);
	close(fd);
	*stop=0;
	while(*stop!='\n')stop++;
	stop++;
	fd=open("etc/variables",O_WRONLY|O_PROTECT);
	write(fd,variable_buf,strlen(variable_buf));
	write(fd,stop,strlen(stop));
	close(fd);
}

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
		if(x) fwritef(1,"global ");
		else fwritef(1,"local ");
		fwritef(1,"variable %s can only be read",name);
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
    envid=atoi(argv[0]);
    int i;
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
    fwritef(1,"\n");
}


#include "lib.h"

int envid;
char variable_buf[4096];

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
			fwritef(1,"can not remove readonly ");
			if(x)
				fwritef(1,"global variable ");
			else
				fwritef(1,"local variable ");
			fwritef(1,"%s",name);
			return;
		}
		remove_variable(stop);
		fwritef(1,"successfully remove ");
		if(x)
			fwritef(1,"global variable ");
		else
			fwritef(1,"local variable ");
		fwritef(1,"%s",name);
	}else{
		fwritef(1,"no ");
		if(px)
		fwritef(1,"global variable named %s",pname);
		else
		fwritef(1,"local variable named %s",pname);
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
		del_variable(name,x);
	}else{
		fwritef(1,"usage: unset name [-x]");
	}
    fwritef(1,"\n");
}
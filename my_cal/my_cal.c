
void _my_getchar();
void _my_putchar(char ch);
void _my_exit();
i
void my_cal()
{
	unsigned int num=0;
	char c;
	while(1){
		c=_my_getchar();
		if(c=='\n')break;
		num=num*10+c-'0';
	}
	char ans[35],l=0;
	while(num>0){
		ans[l++]=num%2;
		num/=2;
	}
	for(int i=l-1;i>=0;i--){
		_my_putchar(ans[i]+'0');
	}
}

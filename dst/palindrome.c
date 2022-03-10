#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);

        int temp1=0,temp2=n;
	while(n>0){
		temp1*=10;
		temp1+=n%10;
		n/=10;
	}
	if(temp1==temp2){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}

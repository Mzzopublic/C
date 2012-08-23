#include <conio.h>
#include <stdio.h>
int isP(int m)
{
	int i;
	for(i=2;i<m;i++)	
		if(m % i==0)return 0;	
		return 1;
}
void num(int m,int k,int xx[])

{ int s=0;

for(m=m+1;k>0;m++)

if(isP(m)) { xx[s++]=m; k--;}
}
main()
{
	int m,n,xx[1000];
	printf("\nPlease enter two integers:");
	scanf("%d%d",&m,&n);
	num(m,n,xx);	
	for(m=0;m<n;m++)		
	printf("%d ",xx[m]);	
	printf("\n");
}

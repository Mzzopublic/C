#include <stdio.h>
void main()
{
	long int a,b,c,i,j,k;
	clrscr();
	for(i=10;i<100;i++)
	{
		for(j=0;j<=9;j++)
			for(k=0;k<=9;k++)
			{
				a=8*100+j*10+k;
				b=97*100+j*10+k;
				if((b==i*a+1)&&b>=1000&&b<=10000&&8*i<100&&9*i>=100)
					printf("\n %ld=%ld*%ld+%ld",97*100+j*10+k,800+j*10+k,i,b%i);
			}
	}
	printf("\n Press any key to quit...");
	getch();
	return;
}
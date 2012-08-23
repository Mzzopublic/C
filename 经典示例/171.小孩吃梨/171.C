#include <stdio.h> /*小孩吃梨问题*/
void main()
{
	long x,y; /* 定义两个变量*/
	x=1;
	y=18;
	clrscr();
	puts("       This program is to solve");
	puts(" The Problem of Child's Eating Pears.");
	while(y>0)
	{
		x=2*(x+1);
		y--;
	}
	printf(" >> In the first day, the child bought %ld pears.\n",x);
	printf(" Press any key to quit...");
	getch();
	return;
}
#include <stdio.h>
#include <conio.h>
int main(void)
{
	int i,j,x,y;
	clrscr();
	printf("\n\n  * * * ³Ë·¨¿Ú¾÷±í * * * \n\n");//?????全是乱码
	x=9;
	y=5;
	for(i=1;i<=9;i++)
	{
		gotoxy(x,y);
		printf("%2d ",i);
		x+=3;
	}
	x=7;
	y=6;
	for(i=1;i<=9;i++)
	{
		gotoxy(x,y);
		printf("%2d ",i);
		y++;
	}
	x=9;
	y= 6;
	for(i=1;i<=9;i++)
	{
		for(j=1;j<=9;j++)
		{
			gotoxy(x,y);
			printf("%2d ",i*j);
			y++;
		}
		y-=9;
		x+=3;
	}
	printf("\n\n");
	return 0;
}//明显语法错误这么多？我还得一个一个给你改






















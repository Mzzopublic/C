/*
路边吃数游戏
宫殿中找出几个数之和是7的数并把它们吃掉，
再走到7的位置就获胜了。
*/
#define UP 0x4800
#define DOWN 0x5000
#define LEFT 0x4b00
#define RIGHT 0x4d00
#include <conio.h>
#include <time.h>
#include <stdlib.h>
int k[17][24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,
0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,
0,3,1,1,1,1,0,1,1,1,4,1,0,1,0,1,1,1,1,1,1,1,0,0,
0,1,0,0,1,0,1,0,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,
0,2,1,1,1,1,6,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,
0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,1,1,1,5,1,1,1,0,1,0,0,1,0,0,0,0,
0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,4,0,0,1,0,0,0,0,
0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1,0,1,1,0,
0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,
0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,3,1,1,1,1,1,0,0,
0,2,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
int en(int *b,int n)
{int i;
for(i=0;i<20;i++)
if(b[i]==n)return 0;
return 1;
}
fu(int *b)
{int i;
for(i=0;i<20;i++)
b[i]=0;
}
void lostgame(int num)
{
	textcolor(WHITE);
	gotoxy(11,22);
	cprintf("The sum of the Numbers you have eaten is: %d.",num);
	gotoxy(11,23);
	cprintf("Sorry, you have lost the game!Press any key to restart...");
	getch();
	gotoxy(11,22);
	cprintf("                                                   ");
	gotoxy(11,23);
	cprintf("                                                         ");
}
void wingame()
{
	textcolor(WHITE);
	gotoxy(11,22);
	cprintf("Yeah! Congratulations! You have won the game!");
	gotoxy(11,23);
	cprintf("Press any key to quit...");
	getch();
	window(1,1,25,80);
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	clrscr();
	exit(0);
}
main()
{
	int i,j,key,num,b[20],p;
 	char ch;
aa: 
	window(1,1,25,80);
	textbackground(LIGHTGREEN);
	textcolor(YELLOW);
	clrscr();
	fu(b);
	num=0;
	p=0;
	textcolor(LIGHTRED);
	gotoxy(18,1);
	cprintf("%c(UP) %c(DOWN) %c(LEFT) %c(RIGHT)",24,25,27,26);
	gotoxy(16,2);
	cprintf("Eat the numbers in your road whose sum is 7,");
	gotoxy(14,3);
	cprintf("and eat 7 in the end, then you will win the game!");
	textcolor(MAGENTA);
	for(i=4;i<21;i++)
		for(j=20;j<44;j++)
			if(k[i-4][j-20]==0)
			{
				gotoxy(j,i);
				cprintf("%c",219);
			}
			else if(k[i-4][j-20]!=1)
			{
				gotoxy(j,i);
				cprintf("%d",k[i-4][j-20]);
			}
	textcolor(YELLOW);
	i=5;
	j=21;
	gotoxy(j,i);
	cprintf("\1");
	gotoxy(33,5);
	cprintf("7");
	gotoxy(7,7);
	cprintf("num:%d",num);
	textcolor(WHITE);
	gotoxy(11,22);
	cprintf("Press any key to start game, Q key to quit...");
	ch=getch();
	if(ch=='Q'||ch=='q')
	{
		window(1,1,25,80);
		textbackground(BLACK);
		textcolor(LIGHTGRAY);
		clrscr();
		exit(0);
	}
	else
	{
		textcolor(YELLOW);
		gotoxy(11,22);
		cprintf("                                             ");
	}
	do
	{
		key=bioskey(0);
		sound(200);
		delay(1000);
		nosound();
		switch(key)
		{
			case DOWN:
			{
				if(k[i-4+1][j-20]==0)
					continue;
				if(k[i-4+1][j-20]!=1&&k[i-4+1][j-20]!=7&&en(b,i+j+1))
				{
					num+=k[i-4+1][j-20];
					b[p++]=i+j+1;
					gotoxy(7,7);
					cprintf("num:%d",num);
				}
				textcolor(YELLOW);
				gotoxy(j,i++);
				printf(" ");
				gotoxy(j,i);
				cprintf("\1");
				gotoxy(33,5);
				if(k[i-4][j-20]==7&&num==7)
					wingame();
				else if(num!=7&&k[i-4][j-20]==7)
				{
					lostgame(num);
					goto aa;
				}
				break; 
			}
			case UP:
			{
				if(k[i-4-1][j-20]==0)
					continue;
				if(k[i-4-1][j-20]!=1&&k[i-4-1][j-20]!=7&&en(b,i+j-1))
				{
					num+=k[i-4-1][j-20];
					b[p++]=i+j-1;
					gotoxy(7,7);
					cprintf("num:%d",num);
				}
				textcolor(YELLOW);
				gotoxy(j,i--);
				printf(" ");
				gotoxy(j,i);
				cprintf("\1");
				gotoxy(33,5);
				if(k[i-4][j-20]==7&&num==7)
					wingame();
				else if(num!=7&&k[i-4][j-20]==7)
				{
					lostgame(num);
					goto aa;
				}
				break; 
			}
			case LEFT:
			{
				if(k[i-4][j-20-1]==0)
					continue;
				if(k[i-4][j-20-1]!=1&&k[i-4][j-20-1]!=7&&en(b,i+j-1))
				{
					num+=k[i-4][j-20-1];
					b[p++]=i+j-1;
					gotoxy(7,7);
					cprintf("num:%d",num);
				}
				textcolor(YELLOW);
				gotoxy(j--,i);
				printf(" ");
				gotoxy(j,i);
				cprintf("\1");
				gotoxy(33,5);
				if(k[i-4][j-20]==7&&num==7)
					wingame();
				else if(num!=7&&k[i-4][j-20]==7)
				{
					lostgame(num);
					goto aa;
				}
				break; 
			}
			case RIGHT:
			{
				if(k[i-4][j-20+1]==0)
					continue;
				if(k[i-4][j-20+1]!=1&&k[i-4][j-20+1]!=7&&en(b,i+j+1))
				{
					num+=k[i-4][j-20+1];
					b[p++]=i+j+1;
					gotoxy(7,7);
					cprintf("num:%d",num);
				}
				textcolor(YELLOW);
				gotoxy(j++,i);
				printf(" ");
				gotoxy(j,i);
				cprintf("\1");gotoxy(33,5);
				if(k[i-4][j-20]==7&&num==7)
					wingame();
				else if(num!=7&&k[i-4][j-20]==7)
				{
					lostgame(num);
					goto aa;
				}
				break; 
			}
			default:
				continue;
		}
	}while(1);
}
 
 
